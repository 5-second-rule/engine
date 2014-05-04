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
protected:


public:
	Event();
	~Event();

	int type;

	virtual void reserveSize(BufferBuilder *buffer);
	virtual void fillBuffer(BufferBuilder *buffer);
	virtual void deserialize(BufferReader& buffer);

	/*bool operator<(Event const&) const;
	bool operator==(Event const&) const;*/
};