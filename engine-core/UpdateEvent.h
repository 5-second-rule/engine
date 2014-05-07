#pragma once
#include "Event.h"

#include "Handle.h"
#include "ISerializable.h"
#include "EventType.h"

class UpdateEvent : public Event
{
private:
	Handle handle;
	ISerializable* child;

public:
	static const EventType TYPE = EventType::UPDATE;

	UpdateEvent(Handle handle, ISerializable* child);
	~UpdateEvent();

	const Handle& getHandle();
	ISerializable* getChild();

	//ISerializable methods
	void reserveSize(IReserve& buffer);
	void fillBuffer(IFill& buffer);

	void deserialize(BufferReader& reader);

};

