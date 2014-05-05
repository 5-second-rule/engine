#pragma once

#include <set>

#include "Handle.h"
#include "IEventReceiver.h"
#include "Event.h"
#include "CrudeTimer.h"
#include "World.h"
using namespace std;
template class COREDLL set< Event >;
class COREDLL World;

const double SEND_EVENT_IMMEDIATELY = 0.0f;
const int NO_ADDITIONAL_INFO = (int)nullptr;

class COREDLL EventDispatcher
{
private:
	set< Event > eventQueue;
	void discharge(Handle &receiver, Event &event);
	World* m_world;

public:
	EventDispatcher(World *p_world);
	~EventDispatcher();
	void EventDispatcher::dispatchDelayedEvents();
	void dispatchEvent(Handle &sender,
		Handle &receiver,
		EventType eventType,
		double delay = SEND_EVENT_IMMEDIATELY,
		void* extraInfo = nullptr);
};