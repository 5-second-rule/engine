#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

#include "engine-core.h"
#include "Socket.h"
#include "DoubleBufferedQueue.h"

#ifdef WIN32
// disable warning about zero-length arrays in MSVC
// exporting STL classes, and strerror security warning
#pragma warning( disable : 4200 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4996 )
#endif

// constants
const string mcastAddr = "224.0.23.189";
const int svrPort = 61616;
const int clntPort = 16161;
const int maxMsgSize = 65507; // max UDP size

using namespace std;

/// <summary>Structure containing the Message header and data</summary>
struct COREDLL Message {
	struct _header {
		uint8_t msgType;
		uint8_t reserved1;
		short reserved2;
	} header;

	uint8_t payload[0];
};

/// <summary>Structure that encapsulated Queued Items </summary>
struct COREDLL QueueItem {
	char *data;
	size_t len;
};

class COREDLL ArgumentException : public exception {
public:
	/// <summary> Construct a ArgumentException with a explanatory message.</summary>
	/// <param name="message">explanatory message</param>
	/// <param name="incSysMsg">true if system message (from strerror(errno)) 
	///  should be postfixed to the user provided message</param>
	ArgumentException( const string &message, bool inclSysMsg = false );

	/// <summary>Deconstructor</summary>
	~ArgumentException() _NOEXCEPT;

	/// <summary>Get the exception message</summary>
	/// <returns>exception message</returns>
	const char *what() const _NOEXCEPT;

private:
	/// <summary>Stores the exception message</summary>
	string userMessage;
};

/// <summary> Not Implemented Exception </summary>
class COREDLL NotImplementedException : public exception {
public:

	/// <summary>Construct a NotImplementedException with a explanatory message.</summary>
	/// <param name="message">explanatory message</param>
	/// <param name="incSysMsg">true if system message (from strerror(errno)) 
	///  should be postfixed to the user provided message</param>
	NotImplementedException( const string &message, bool inclSysMsg = false );

	/// <summary>Deconstructor</summary>
	~NotImplementedException() _NOEXCEPT;

	/// <summary>Get the exception message</summary>
	/// <returns>exception message</returns>
	const char *what() const _NOEXCEPT;

private:
	/// <summary>Stores the exception message</summary>
	string userMessage;
};


/// <summary>Role Type Enumerations</summary>
enum class COREDLL CommsProcessorRole {
	SERVER,
	CLIENT,
	LOOPBACK,
	MONITOR,
	CUSTOM
};


/// <summary>Message Type Enumerations</summary>
enum class COREDLL MessageType {
	WORLD_UPDATE,
	CLIENT_UPDATE,
	SERVER_ANNOUNCE
};

/// <summary>CommsProcessor class handle sending and recieving messages</summary>
class COREDLL CommsProcessor {
public:
	/// <summary>Constructor</summary>
	/// <param name="role">The role of this CommsProcessor object</param>
	CommsProcessor( CommsProcessorRole role );

	/// <summary>Deconstructor</summary>
	~CommsProcessor();

	/// <summary>Sets the queue to handoff client or server updates too</summary>
	/// <param name="q">The queue to use</param>
	void setHandoffQ( DoubleBufferedQueue<QueueItem> *q );

	/// <summary>Gets the currently assigned queue to handoff client or server updates too</summary>
	/// <returns>The currently assigned handoff queue</returns>
	DoubleBufferedQueue<QueueItem> *getHandoffQ() const;

	/// <summary>Sends a message</summary>
	/// <param name="data">the byte stream representing a Message</param>
	/// <param name="len">the length of the byte stream to send</param>
	void sendUpdates( const char *data, size_t len );

	/// <summary>Sends a server announce message</summary>
	/// <exception cref="NotImplementedException">Thrown when an the role is Custom</exception>
	/// <exception cref="ArgumentException">Thrown when an the role is unknown</exception>
	void sendAnnouce();

	/// <summary>Blocks until an announce message is recieved from a server</summary>
	void waitAnnouce();

private:

	// listen thread related
	thread listenThread;
	char recvBuf[maxMsgSize];
	bool running;
	void serverCallback();
	void clientCallback();
	void loopbackCallback();
	void monitorCallback();

	// comms role
	CommsProcessorRole role;

	// handoffQ private pointer
	DoubleBufferedQueue<QueueItem> *handoffQ;

	// send update related
	UDPSocket sendSocket;
	UDPSocket listenSocket;
	char sendBuf[maxMsgSize];

	mutex m;
	condition_variable cv;
	bool announceSignaled;
	string serverAddr;
};
