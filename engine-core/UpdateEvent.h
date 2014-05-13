#pragma once
#include "Event.h"

#include "Handle.h"
#include "BaseObject.h"
#include "EventType.h"

class UpdateEvent : public Event
{
private:
	Handle handle;
	BaseObject* child;

public:
	static const EventType TYPE = EventType::UPDATE;

	UpdateEvent(Handle handle, BaseObject* child);
	~UpdateEvent();

	const Handle& getHandle();
	BaseObject* getChild();
	void setChild( BaseObject* child);

	//ISerializable methods
	void reserveSize(IReserve& buffer) const;
	void fillBuffer(IFill& buffer) const;
	void deserialize(BufferReader& reader);
};

