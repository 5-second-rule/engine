#include "Event.h"


Event::Event(){

}

Event::~Event()
{
}

Event::Event(Handle &sender, Handle &receiver, EventType type, void* extranInfo)
{
	this->sender = sender;
	this->receiver = receiver;
	this->type = type;
	this->dispatchTime = high_resolution_clock::now();

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

void Event::setDispatchTime(high_resolution_clock::time_point &time){
	dispatchTime = time;
}

high_resolution_clock::time_point Event::getDispatchTime() const{
	return dispatchTime;
}

Handle &Event::getReceiver(){
	return receiver;
}