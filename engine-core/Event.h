#pragma once

#include "engine-core.h"
#include "EventType.h"
#include "Handle.h"
#include "ISerializable.h"

struct COREDLL EventHeader {
	int type;
};

class COREDLL Event : public ISerializable
{
private:
	/* TODO: const */EventType type;
	Event(); // intentionally private
public:
	Event(EventType type);
	virtual ~Event();

	virtual void reserveSize(IReserve& buffer) const;
	virtual void fillBuffer(IFill& buffer) const;
	virtual void deserialize(BufferReader& buffer);

	EventType getType();

	template<class T> static T* cast(Event* e) {
		if (T::TYPE == e->getType()) return static_cast<T*>(e);
		else return nullptr;
	}

	static EventType getType(BufferReader&);
};
