#pragma once

#include "engine-core.h"
#include "Event.h"

class COREDLL IEventReceiver {
public:
	virtual void enqueue(Event *evt) = 0;
};