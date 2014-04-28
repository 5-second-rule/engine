//#pragma once
//
//#include "Handle.h"
//#include "IRecentReceiver.h"
//#include "Event.h"
//#include "CrudeTimer.h"
//#include <set>
//using namespace std;
//template class COREDLL set< Event >;
//
//const double SEND_EVENT_IMMEDIATELY = 0.0f;
//const int NO_ADDITIONAL_INFO = (int)nullptr;
//
//class EventDispatcher
//{
//private:
//	set< Event > eventQueue;
//
//	void discharge(Handle &receiver, Event &event);
//	
//	
//public:
//	EventDispatcher();
//	~EventDispatcher();
//	void EventDispatcher::dispatchDelayedEvents();
//	void dispatchEvent(	Handle &sender,
//						Handle &receiver,
//						EventType eventType,
//						double delay = SEND_EVENT_IMMEDIATELY,
//						void* extraInfo = nullptr);
//};
//
