#include "EventDispatcher.h"

EventDispatcher::~EventDispatcher()
{
}

void EventDispatcher::discharge(Handle &receiver, Event &event){
	/* TODO */
	IHasHandle *object = nullptr; // <--- Need a way to retrieve the object pointer from World = receiver.toObject();
	IEventReceiver *object_receiver = dynamic_cast<IEventReceiver*>(object);

	if (object_receiver != nullptr) {
		object_receiver->onEvent(event);
	}
	else {
		// TODO: throw an exception
	}
}

void EventDispatcher::dispatchEvent(high_resolution_clock::duration delay,
	Handle &sender,
	Handle &receiver,
	EventType eventType,
	void* extraInfo)
{
	Event event(sender, receiver, eventType, extraInfo);
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();

	if (delay == std::chrono::duration< int >(0)){
		discharge(receiver, event);
	}
	else {
		event.setDispatchTime(high_resolution_clock::now() + delay);
		eventQueue.insert(event);
	}
}

void EventDispatcher::dispatchDelayedEvents(){
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();

	while ((eventQueue.begin() != eventQueue.end()) &&
		(eventQueue.begin()->getDispatchTime() < currentTime)){
		Event event = *eventQueue.begin();

		discharge(event.getReceiver(), event);

		eventQueue.erase(eventQueue.begin());
	}
}