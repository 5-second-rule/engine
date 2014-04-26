#pragma once

#include "engine-core.h"
#include "IHasHandle.h"
#include "Event.h"

class COREDLL IEventReceiver : public IHasHandle {
public:
	virtual bool onEvent(Event event) = 0;
};