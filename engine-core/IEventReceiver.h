#pragma once

#include "engine-core.h"
#include "DirectedEvent.h"

class COREDLL IEventReceiver {
public:
	virtual bool onEvent(DirectedEvent *evt) = 0;
};