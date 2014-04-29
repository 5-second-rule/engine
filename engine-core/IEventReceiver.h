#pragma once

#include "engine-core.h"
#include "DirectedEvent.h"

class COREDLL IEventReceiver {
public:
	virtual void onEvent(DirectedEvent *evt) = 0;
};