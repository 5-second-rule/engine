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

protected:
	virtual void dehydrateInternal(BufferBuilder *buffer);

public:
	DirectedEvent(
		int eventType, 
		Handle &sender,
		Handle &receive,
		ISerializable *child);

	~DirectedEvent();

	Handle &getReceiver();

	virtual void rehydrate(BufferBuilder *buffer);

	static DirectedEvent *forReading() {
		DirectedEvent *event = new DirectedEvent();
		event->child = new BufferRemainder();
		return event;
	}
};

