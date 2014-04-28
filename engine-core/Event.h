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

protected:
	// HACK
	virtual void dehydrateInternal(BufferBuilder *buffer);

public:
	Event(EventType type);
	~Event();

	virtual void dehydrate(BufferBuilder *buffer);
	virtual void rehydrate(BufferBuilder *buffer);

	/*bool operator<(Event const&) const;
	bool operator==(Event const&) const;*/
};