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
	EventType type;

public:
	Event(EventType type);
	~Event();

	virtual void reserveSize(IReserve& buffer);
	virtual void fillBuffer(IFill& buffer);
	virtual void deserialize(BufferReader& buffer);

	/*bool operator<(Event const&) const;
	bool operator==(Event const&) const;*/
};