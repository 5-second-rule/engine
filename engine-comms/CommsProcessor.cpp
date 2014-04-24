#include "CommsProcessor.h"

/**
*   Argument Exception
*/
ArgumentException::ArgumentException( const string &message, bool inclSysMsg ) : userMessage( message )
{
	if( inclSysMsg )
	{
		userMessage.append( ": " );
		userMessage.append( strerror( errno ) );
	}
}

ArgumentException::~ArgumentException() _NOEXCEPT
{
}

const char *ArgumentException::what() const _NOEXCEPT
{
	return userMessage.c_str();
}

/**
*   Not Implemented Exception
*/
NotImplementedException::NotImplementedException( const string &message, bool inclSysMsg ) : userMessage( message )
{
	if( inclSysMsg )
	{
		userMessage.append( ": " );
		userMessage.append( strerror( errno ) );
	}
}

NotImplementedException::~NotImplementedException() _NOEXCEPT
{
}

const char *NotImplementedException::what() const _NOEXCEPT
{
	return userMessage.c_str();
}

/**
*   CommsProcessor class
*/
CommsProcessor::CommsProcessor( CommsProcessorRole r )
{
	//TODO Make TTL variable and accessors instead of hardcoded
	role = r;
	sendSocket.setMulticastTTL( 255 );
	running = true;
	announceSignaled = false;
	switch( r )
	{
		case SERVER:
			listenThread = thread( &CommsProcessor::serverCallback, this );
			break;
		case CLIENT:
			listenThread = thread( &CommsProcessor::clientCallback, this );
			break;
		case MONITOR:
			listenThread = thread( &CommsProcessor::monitorCallback, this );
			break;
		case CUSTOM:
			throw NotImplementedException( "Custom is not yet implemented" );
		default:
			throw ArgumentException( "An invalid mode was specified" );
	}
}

CommsProcessor::~CommsProcessor()
{
	running = false;
	listenThread.join();
}

void CommsProcessor::setHandoffQ( queue<QueueItem> *q )
{
	if( q )
	{
		handoffQ = q;
	}
	else
	{
		throw ArgumentException( "MessageCallback was attempted to be set to NULL" );
	}
}

queue<QueueItem> *CommsProcessor::getHandoffQ()
{
	return handoffQ;
}

void CommsProcessor::serverCallback()
{
	string sourceAddress;								// Address of datagram source
	unsigned short sourcePort;					// Port of datagram source
	UDPSocket listenSocket( svrPort );

	// set up the listening socket
	listenSocket.joinGroup( mcastAddr );
	listenSocket.setRecvTimeout( 500 ); // 500 ms

	while( running )
	{
		int bytesRcvd;

		bytesRcvd = listenSocket.recvFrom( recvBuf, maxMsgSize, sourceAddress, sourcePort );
	
		// if recieve call timedout then just start loop over
		if( bytesRcvd == 0 )
			continue;

		Message *buf = (Message*)recvBuf;
		switch( buf->header.msgType )
		{
			case CLIENT_UPDATE:
			{
				QueueItem item;
				item.data = new char[bytesRcvd - sizeof( Message )];
				memcpy( item.data, buf->payload, bytesRcvd - sizeof( Message ) );
				item.len = bytesRcvd - sizeof( Message );
				handoffQ->push( item );
			}
			default:
				break;
		}
	}

	listenSocket.leaveGroup( mcastAddr );
}

void CommsProcessor::clientCallback()
{
	string sourceAddress;								// Address of datagram source
	unsigned short sourcePort;					// Port of datagram source
	UDPSocket listenSocket( clntPort );

	// set up the listening socket
	listenSocket.joinGroup( mcastAddr );
	listenSocket.setRecvTimeout( 500 ); // 500 ms

	while( running )
	{
		int bytesRcvd;

		bytesRcvd = listenSocket.recvFrom( recvBuf, maxMsgSize, sourceAddress, sourcePort );

		// if recieve call timedout then just start loop over
		if( bytesRcvd == 0 )
			continue;

		Message *buf = (Message*)recvBuf;
		switch( buf->header.msgType )
		{
			case WORLD_UPDATE:
			{
				QueueItem item;
				item.data = new char[bytesRcvd - sizeof( Message )];
				memcpy( item.data, buf->payload, bytesRcvd - sizeof( Message ) );
				item.len = bytesRcvd - sizeof( Message );
				handoffQ->push( item );
			}
			case SERVER_ANNOUNCE:
				announceSignaled = true;
				cv.notify_one();
				break;
			default:
				break;
		}
	}

	listenSocket.leaveGroup( mcastAddr );
}

void CommsProcessor::monitorCallback()
{

}

void CommsProcessor::sendUpdates( const char *data, size_t len )
{
	if( len > (maxMsgSize - sizeof(Message) ) )
	{
		throw ArgumentException( "Message len is too big" );
	}

	int port;
	Message *buf = (Message*)sendBuf;
	memcpy( buf->payload, data, len );
	buf->header.reserved1 = 0;
	buf->header.reserved2 = 0;

	switch( role )
	{
		case SERVER:
			buf->header.msgType = WORLD_UPDATE;
			port = clntPort;
			break;
		case CLIENT:
			buf->header.msgType = CLIENT_UPDATE;
			port = svrPort;
			break;
		case MONITOR:
			return;
		case CUSTOM:
			throw NotImplementedException( "Custom is not yet implemented" );
		default:
			throw ArgumentException( "An invalid mode was specified" );
	}

	sendSocket.sendTo( buf, sizeof( Message ) + len, mcastAddr, port );
}

void CommsProcessor::sendAnnouce()
{
	Message buf;
	buf.header.msgType = SERVER_ANNOUNCE;
	buf.header.reserved1 = 0;
	buf.header.reserved2 = 0;

	switch( role )
	{
		case SERVER:
			break;
		case CLIENT:
			return;
		case MONITOR:
			return;
		case CUSTOM:
			throw NotImplementedException( "Custom is not yet implemented" );
		default:
			throw ArgumentException( "An invalid mode was specified" );
	}

	sendSocket.sendTo( &buf, sizeof( Message ), mcastAddr, clntPort );
}

void CommsProcessor::waitAnnouce()
{
	// Wait until main() sends data
	std::unique_lock<std::mutex> lk( m );
	cv.wait( lk, [=] { return announceSignaled; } );
}
