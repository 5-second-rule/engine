#pragma once

#include "engine-core.h"
#include "EventType.h"
#include "Handle.h"
#include "ISerializable.h"

struct COREDLL EventHeader {
	int type;
};

//struct COREDLL DirectedEventHeader {
//	Handle sender;
//	Handle receiver;
//};

class COREDLL Event : public ISerializable
{
private:
	EventType type;
	//Handle sender;
	//Handle receiver;
	//ISerializable *child;

public:
	/*Event(Handle &sender,
		Handle &receiver,
		EventType type,
		ISerializable* extraInfo);*/
	Event(EventType type);
	~Event();

	//Handle &getReceiver();

	virtual void dehydrate(BufferBuilder *buffer);
	virtual void rehydrate(BufferBuilder *buffer);

	/*bool operator<(Event const&) const;
	bool operator==(Event const&) const;*/
};