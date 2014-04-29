#pragma once

#include "Event.h"
#include "BufferRemainder.h"

struct COREDLL DirectedEventHeader {
	Handle sender;
	Handle receiver;
};

class COREDLL DirectedEvent : public Event
{
private:
	Handle sender;
	Handle receiver;
	ISerializable *child;

	DirectedEvent();

public:
	DirectedEvent(
		int eventType, 
		Handle &sender,
		Handle &receive,
		ISerializable *child);

	~DirectedEvent();

	Handle &getReceiver();

	virtual void reserveSize(BufferBuilder *buffer);
	virtual void fillBuffer(BufferBuilder *buffer);
	virtual void deserialize(BufferReader *buffer);

	static DirectedEvent *forReading();
};
