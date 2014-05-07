#pragma once

#include "ConstructorTable.h"
#include "Event.h"

class EventFactory : public ConstructorTable<Event>
{
public:
	EventFactory(size_t size);
	~EventFactory();
};

