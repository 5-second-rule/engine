#pragma once
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Socket.h"

#ifdef WIN32
// disable warning about zero-length arrays in MSVC
#pragma warning( disable : 4200 )
#ifdef _IS_ENGINE_CORE
#define COMMSDLL __declspec(dllexport)
#else
#define COMMSDLL __declspec(dllimport)
#endif
#elif
#define COMMSDLL
#endif

using namespace std;

/**
*   Message Header Struct
*/
struct Message
{
	struct _header
	{
		uint8_t msgType;
		uint8_t reserved1;
		short reserved2;
	} header;

	uint8_t payload[0];
};

/**
*   Queue Header Struct
*/
struct QueueItem
{
	char *data;
	size_t len;
};

/**
*   Argument Exception
*/
class COMMSDLL ArgumentException : public exception
{
public:
	/**
	*   Construct a ArgumentException with a explanatory message.
	*   @param message explanatory message
	*   @param incSysMsg true if system message (from strerror(errno))
	*   should be postfixed to the user provided message
	*/
	ArgumentException( const string &message, bool inclSysMsg = false );

	/**
	*   Provided just to guarantee that no exceptions are thrown.
	*/
	~ArgumentException() _NOEXCEPT;

	/**
	*   Get the exception message
	*   @return exception message
	*/
	const char *what() const _NOEXCEPT;

private:
	string userMessage;  // Exception message
};


/**
*   Not Implemented Exception
*/
class NotImplementedException : public exception
{
public:
	/**
	*   Construct a NotImplementedException with a explanatory message.
	*   @param message explanatory message
	*   @param incSysMsg true if system message (from strerror(errno))
	*   should be postfixed to the user provided message
	*/
	NotImplementedException( const string &message, bool inclSysMsg = false );

	/**
	*   Provided just to guarantee that no exceptions are thrown.
	*/
	~NotImplementedException() _NOEXCEPT;

	/**
	*   Get the exception message
	*   @return exception message
	*/
	const char *what() const _NOEXCEPT;

private:
	string userMessage;  // Exception message
};


/**
 * Support Enumerations
 */
enum CommsProcessorRole
{
	SERVER,
	CLIENT,
	MONITOR,
	CUSTOM
};

enum MessageType
{
	WORLD_UPDATE,
	CLIENT_UPDATE,
	SERVER_ANNOUNCE
};

/**
*   CommsProcessor class
*/
class CommsProcessor
{
public:
  // object lifecycle methods
	CommsProcessor( CommsProcessorRole role );
  ~CommsProcessor();

	// configuration methods
	void setHandoffQ( queue<QueueItem> *q );
	queue<QueueItem> *getHandoffQ();

	// Sending methods
	void sendUpdates( const char *data, size_t len );
	void sendAnnouce();

	// waiting functions
	void waitAnnouce();

private:
	// constants
	const string mcastAddr = "224.0.23.189";
	static const int svrPort = 61616;
	static const int clntPort = 16161;
	static const int maxMsgSize = 65507; // max UDP size

	// listen thread related
	thread listenThread;
	char recvBuf[maxMsgSize];
	bool running;
	void serverCallback();
	void clientCallback();
	void monitorCallback();

	// comms role
	CommsProcessorRole role;

	// handoffQ private pointer
	queue<QueueItem> *handoffQ;

	// send update related
	UDPSocket sendSocket;
	char sendBuf[maxMsgSize];

	mutex m;
	condition_variable cv;
	bool announceSignaled;
};
