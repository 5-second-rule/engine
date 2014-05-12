#include "CommsProcessor.h"

// Argument Exception
ArgumentException::ArgumentException( const string &message, bool inclSysMsg ) : userMessage( message ) {
	if( inclSysMsg ) {
		userMessage.append( ": " );
		userMessage.append( strerror( errno ) );
	}
}

ArgumentException::~ArgumentException() _NOEXCEPT {}

const char *ArgumentException::what() const _NOEXCEPT {
	return userMessage.c_str();
}

// Not Implemented Exception
NotImplementedException::NotImplementedException( const string &message, bool inclSysMsg ) : userMessage( message ) {
	if( inclSysMsg ) {
		userMessage.append( ": " );
		userMessage.append( strerror( errno ) );
	}
}

NotImplementedException::~NotImplementedException() _NOEXCEPT {}

const char *NotImplementedException::what() const _NOEXCEPT {
	return userMessage.c_str();
}

// CommsProcessor class
CommsProcessor::CommsProcessor( CommsProcessorRole r, Engine* owner ) {
	//TODO Make TTL variable and accessors instead of hardcoded
	this->owner = owner;
	this->role = r;
	this->sendSocket.setMulticastTTL( 1 );
	this->running = true;
	this->announceSignaled = false;
	this->serverAddr = "255.255.255.255";
	switch( r ) {
		case CommsProcessorRole::SERVER:
			this->listenThread = thread( &CommsProcessor::serverCallback, this );
			break;
		case CommsProcessorRole::CLIENT:
			this->listenThread = thread( &CommsProcessor::clientCallback, this );
			break;
		case CommsProcessorRole::MONITOR:
			this->listenThread = thread( &CommsProcessor::monitorCallback, this );
			break;
		case CommsProcessorRole::CUSTOM:
			throw NotImplementedException( "Custom is not yet implemented" );
		default:
			throw ArgumentException( "An invalid mode was specified" );
	}
}

CommsProcessor::~CommsProcessor() {
	this->running = false;
	this->listenThread.join();
}

void CommsProcessor::setHandoffQ( DoubleBufferedQueue<Event*> *q ) {
	if( q ) {
		this->handoffQ = q;
	} else {
		throw ArgumentException( "Message queue was attempted to be set to nullptr" );
	}
}

DoubleBufferedQueue<Event*> *CommsProcessor::getHandoffQ() const {
	return this->handoffQ;
}

void CommsProcessor::serverCallback() {
	string sourceAddress;								// Address of datagram source
	unsigned short sourcePort;					// Port of datagram source
	listenSocket.setLocalPort( svrPort );

	// set up the listening socket
	this->listenSocket.setRecvTimeout( 500 ); // 500 ms

	while( running ) {
		int bytesRcvd;

		bytesRcvd = this->listenSocket.recvFrom( recvBuf, maxMsgSize, sourceAddress, sourcePort );

		// if recieve call timedout then just start loop over
		if( bytesRcvd == 0 )
			continue;

		Message *buf = (Message*)recvBuf;
		switch( MessageType(buf->header.msgType) ) {
			case MessageType::CLIENT_EVENT:
			{
				BufferReader readBuffer( buf->payload, buf->header.len);
				EventType type = Event::getType( readBuffer );
				Event* event;
				switch( type ) {
					case EventType::ACTION:
						ActionArgs args;
						args.actionType = ActionEvent::getActionType( readBuffer );
						event = owner->eventCtors->invoke( EventType::ACTION, &args );
						break;
					case EventType::REGISTRATION:
						event = new RegistionEvent();
						break;
					default:
						throw runtime_error( "Something went wrong, this is an unkown event type being deserialized" );
						break;
				}
				event->deserialize( readBuffer );
				handoffQ->push( event );
				break;
			}
			default:
				break;
		}
	}
}

void CommsProcessor::clientCallback() {
	string sourceAddress;								// Address of datagram source
	unsigned short sourcePort;					// Port of datagram source
	listenSocket.setLocalPort( clntPort );

	// set up the listening socket
	listenSocket.joinGroup( mcastAddr );
	listenSocket.setRecvTimeout( 500 ); // 500 ms

	while( running ) {
		int bytesRcvd;

		bytesRcvd = listenSocket.recvFrom( recvBuf, maxMsgSize, sourceAddress, sourcePort );

		// if recieve call timedout then just start loop over
		if( bytesRcvd == 0 )
			continue;

		Message *buf = (Message*)recvBuf;
		switch( MessageType(buf->header.msgType) ) {
			case MessageType::SERVER_EVENT:
			{
				BufferReader readBuffer( buf->payload, buf->header.len );
				EventType type = Event::getType( readBuffer );
				Event* event;
				switch( type ) {
					case EventType::UPDATE:
					{
						event = new UpdateEvent( Handle(), nullptr );
						event->deserialize( readBuffer );
						UpdateEvent* actionEvt = static_cast<UpdateEvent*>(event);
						IHasHandle* obj = owner->objectCtors->invoke( actionEvt->childType, nullptr );
						actionEvt->setChild( dynamic_cast<ISerializable*>(obj));
						actionEvt->getChild()->deserialize( readBuffer );
						break;
					}
					case EventType::REGISTRATION:
						event = new RegistionEvent();
						event->deserialize( readBuffer );
						break;
					default:
						throw runtime_error( "Something went wrong, this is an unkown event type that was recieved" );
						break;
				}
				handoffQ->push( event );
				break;
			}
			case MessageType::SERVER_ANNOUNCE:
				announceSignaled = true;
				if( serverAddr == "255.255.255.255" ) {
					serverAddr = sourceAddress;
				}
				cv.notify_one();
				break;
			default:
				break;
		}
	}

	listenSocket.leaveGroup( mcastAddr );
}

void CommsProcessor::monitorCallback() {}

void CommsProcessor::sendEvent( const Event* evt ) {
	
	BufferBuilder buffer;
	evt->serialize( buffer );
	
	if( buffer.getSize() > (maxMsgSize - sizeof( Message )) ) {
		throw ArgumentException( "Message len is too big" );
	}

	int port;
	string dstAddr;
	Message *buf = (Message*)sendBuf;
	memcpy( buf->payload, buffer.getBasePointer(), buffer.getSize() );
	buf->header.reserved1 = 0;
	buf->header.len = buffer.getSize();

	switch( role ) {
		case CommsProcessorRole::SERVER:
			buf->header.msgType = static_cast<uint8_t>(MessageType::SERVER_EVENT);
			dstAddr = mcastAddr;
			port = clntPort;
			break;
		case CommsProcessorRole::CLIENT:
			buf->header.msgType = static_cast<uint8_t>(MessageType::CLIENT_EVENT);
			dstAddr = serverAddr;
			port = svrPort;
			break;
		case CommsProcessorRole::MONITOR:
			return;
		case CommsProcessorRole::CUSTOM:
			throw NotImplementedException( "Custom is not yet implemented" );
		default:
			throw ArgumentException( "An invalid mode was specified" );
	}

	sendSocket.sendTo( buf, sizeof( Message ) + buffer.getSize(), dstAddr, port );
}

void CommsProcessor::sendAnnouce() {
	Message buf;
	buf.header.msgType = static_cast<uint8_t>(MessageType::SERVER_ANNOUNCE);
	buf.header.reserved1 = 0;
	buf.header.len = 0;

	switch( role ) {
		case CommsProcessorRole::SERVER:
			break;
		case CommsProcessorRole::CLIENT:
			return;
		case CommsProcessorRole::MONITOR:
			return;
		case CommsProcessorRole::CUSTOM:
			throw NotImplementedException( "Custom is not yet implemented" );
		default:
			throw ArgumentException( "An invalid mode was specified" );
	}

	sendSocket.sendTo( &buf, sizeof( Message ), mcastAddr, clntPort );
}

void CommsProcessor::waitAnnouce() {
	std::unique_lock<std::mutex> lk( m );
	cv.wait( lk, [=] { return announceSignaled; } );
}
