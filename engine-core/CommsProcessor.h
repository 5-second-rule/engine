#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

#include "engine-core.h"
#include "Engine.h"
#include "Socket.h"
#include "DoubleBufferedQueue.h"
#include "Event.h"
#include "UpdateEvent.h"
#include "RegistionEvent.h"

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

class Engine;

/// <summary>Structure containing the Message header and data</summary>
struct COREDLL Message {
	struct _header {
		uint8_t msgType;
		uint8_t reserved1;
		uint16_t len;
	} header;

	char payload[0];
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
	SERVER_EVENT,
	CLIENT_EVENT,
	SERVER_ANNOUNCE
};

/// <summary>CommsProcessor class handle sending and recieving messages</summary>
class COREDLL CommsProcessor {
public:
	/// <summary>Constructor</summary>
	/// <param name="role">The role of this CommsProcessor object</param>
	CommsProcessor( CommsProcessorRole role, Engine* owner );

	/// <summary>Deconstructor</summary>
	~CommsProcessor();

	/// <summary>Sets the queue to handoff client or server updates too</summary>
	/// <param name="q">The queue to use</param>
	void setHandoffQ( DoubleBufferedQueue<Event*> *q );

	/// <summary>Gets the currently assigned queue to handoff client or server updates too</summary>
	/// <returns>The currently assigned handoff queue</returns>
	DoubleBufferedQueue<Event*> *getHandoffQ() const;

	/// <summary>Sends an event</summary>
	/// <param name="evt">the the event to send</param>
	void sendEvent( const Event* evt );

	/// <summary>Sends a server announce message</summary>
	/// <exception cref="NotImplementedException">Thrown when an the role is Custom</exception>
	/// <exception cref="ArgumentException">Thrown when an the role is unknown</exception>
	void sendAnnouce();

	/// <summary>Blocks until an announce message is recieved from a server</summary>
	void waitAnnouce();

private:
	// owning Engine object
	Engine* owner;

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
	DoubleBufferedQueue<Event*> *handoffQ;

	// send update related
	UDPSocket sendSocket;
	UDPSocket listenSocket;
	char sendBuf[maxMsgSize];

	mutex m;
	condition_variable cv;
	bool announceSignaled;
	string serverAddr;
};
