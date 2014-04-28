#include "Event.h"

Event::~Event()
{
}

Event::Event(EventType type)
{
	//this->sender = sender;
	//this->receiver = receiver;
	this->type = type;
	//this->child = child;
}
//
//bool Event::operator<(Event const& event) const{
//	if (event == *this)
//		return false;
//	else
//		return event.dispatchTime < this->dispatchTime;
//}
//
//bool Event::operator==(Event const& event) const{
//	return (this->dispatchTime == event.dispatchTime &&
//		(event.sender == this->sender) &&
//		(event.receiver == this->receiver) &&
//		(event.type == this->type));
//}
//
//void Event::setDispatchTime(double time){
//	dispatchTime = time;
//}
//
//double Event::getDispatchTime() const{
//	return dispatchTime;
//}

//Handle &Event::getReceiver(){
//	return receiver;
//}

//void Event::dehydrate(BufferBuilder *buffer) {
//	buffer->reserve(sizeof(struct EventHeader));
//
//	// must call allocate
//	if (this->child != nullptr) {
//		this->child->dehydrate(buffer);
//	}
//
//	struct EventHeader *hdr = reinterpret_cast<struct EventHeader *>(buffer->getPointer());
//
//	hdr->type = this->type;
//	hdr->sender = this->sender;
//	hdr->receiver = this->receiver;
//
//	buffer->pop();
//}
//
//void Event::rehydrate(BufferBuilder *buffer) {
//	buffer->reserve(sizeof(struct EventHeader));
//
//	if (this->child != nullptr) {
//		this->child->rehydrate(buffer);
//	}
//
//	struct EventHeader *hdr = reinterpret_cast<struct EventHeader *>(buffer->getPointer());
//
//	this->type = (EventType)hdr->type;
//	this->sender = hdr->sender;
//	this->receiver = hdr->receiver;
//
//	// no pop
//}

void Event::dehydrateInternal(BufferBuilder *buffer) {
}


void Event::dehydrate(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct EventHeader));

	// HACK right now, make better abstraction
	this->dehydrateInternal(buffer);

	struct EventHeader *hdr = reinterpret_cast<struct EventHeader *>(buffer->getPointer());

	hdr->type = this->type;

	buffer->pop();
}

void Event::rehydrate(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct EventHeader));

	struct EventHeader *hdr = reinterpret_cast<struct EventHeader *>(buffer->getPointer());

	this->type = (EventType)hdr->type;

	// no pop
}