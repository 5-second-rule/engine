#pragma once

#include "Handle.h"
#include "IRecentReceiver.h"
#include "Event.h"
#include <set>
using namespace std;
template class COREDLL set< Event >;

class EventDispatcher
{
private:
	set< Event > eventQueue;

	void discharge(Handle &receiver, Event &event);
	
	
public:
	EventDispatcher();
	~EventDispatcher();
	void EventDispatcher::dispatchDelayedEvents();
	void dispatchEvent(	high_resolution_clock::duration delay,
						Handle &sender,
						Handle &receiver,
						EventType eventType,
						void* extraInfo);
};

