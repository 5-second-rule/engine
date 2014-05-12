#pragma once
#include "Event.h"

#include "Handle.h"
#include "ISerializable.h"
#include "EventType.h"


struct UpdateArgs : Args {
	Handle handle;
	ISerializable* child;
};

class UpdateEvent : public Event
{
private:
	Handle handle;
	ISerializable* child;

public:
	static const EventType TYPE = EventType::UPDATE;
	int childType;

	UpdateEvent(Handle handle, ISerializable* child);
	~UpdateEvent();

	const Handle& getHandle();
	ISerializable* getChild();
	void setChild( ISerializable* child);

	//ISerializable methods
	void reserveSize(IReserve& buffer) const;
	void fillBuffer(IFill& buffer) const;
	void deserialize(BufferReader& reader);
};

