#include "Event.h"

const double Event::SmallestDelay = 0.02f;

Event::~Event()
{
}

Event::Event(double time, Handle &sender, Handle &receiver, EventType type, void* extranInfo)
{
	this->sender = sender;
	this->receiver = receiver;
	this->type = type;
	this->dispatchTime = time;

	switch (type)
	{
	case Event_Attack:
		break;
		this->extraInfo = (EventAttack*)extraInfo;
	case Event_Move:
		this->extraInfo = (EventMove*)extraInfo;
		break;
	default:
		this->extraInfo = nullptr;
		break;
	}
}

bool Event::operator<(Event const& event) const{
	if (event == *this)
		return false;
	else
		return event.dispatchTime < this->dispatchTime;
}

bool Event::operator==(Event const& event) const{
	return (this->dispatchTime == event.dispatchTime &&
		(event.sender == this->sender) &&
		(event.receiver == this->receiver) &&
		(event.type == this->type));
}

void Event::setDispatchTime(double time){
	dispatchTime = time;
}

double Event::getDispatchTime() const{
	return dispatchTime;
}

Handle &Event::getReceiver(){
	return receiver;
}