//#include "EventDispatcher.h"
//
//EventDispatcher::~EventDispatcher()
//{
//}
//
//void EventDispatcher::discharge(Handle &receiver, Event &event){
//	/* TODO */
//	IHasHandle *object = nullptr; // <--- Need a way to retrieve the object pointer from World = receiver.toObject();
//	IEventReceiver *object_receiver = dynamic_cast<IEventReceiver*>(object);
//
//	if (object_receiver != nullptr) {
//		object_receiver->onEvent(event);
//	}
//	else {
//		// TODO: throw an exception
//	}
//}
//
//void EventDispatcher::dispatchEvent(
//	Handle &sender,
//	Handle &receiver,
//	EventType eventType,
//	double delay,
//	void* extraInfo)
//{
//	Event event(delay, sender, receiver, eventType, extraInfo);
//	double currentTime;
//
//	if (delay == SEND_EVENT_IMMEDIATELY){
//		discharge(receiver, event);
//	}
//	else {
//		currentTime = Clock->GetCurrentTime();
//		event.dispatchTime = currentTime + delay;
//		eventQueue.insert(event);
//	}
//}
//
//void EventDispatcher::dispatchDelayedEvents(){
//	double currentTime = Clock->GetCurrentTime();
//
//	while ((eventQueue.begin() != eventQueue.end()) &&
//		(eventQueue.begin()->dispatchTime < currentTime)){
//		Event event = *eventQueue.begin();
//
//		discharge(event.receiver, event);
//
//		eventQueue.erase(eventQueue.begin());
//	}
//}