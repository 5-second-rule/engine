#include "DirectedEvent.h"


DirectedEvent::DirectedEvent(
	int eventType,
	Handle &sender,
	Handle &receiver,
	ISerializable *child)
		: Event((EventType)eventType) {
	this->sender = sender;
	this->receiver = receiver;
	this->child = child;
}

DirectedEvent::DirectedEvent() : Event((EventType)0) {
}


DirectedEvent::~DirectedEvent() {
	if (this->child != nullptr) {
		delete this->child;
	}
}

void DirectedEvent::dehydrateInternal(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct EventHeader));

	if (this->child != nullptr) {
		this->child->dehydrate(buffer);
	}

	struct DirectedEventHeader *hdr = reinterpret_cast<struct DirectedEventHeader *>(buffer->getPointer());

	hdr->receiver = this->receiver;
	hdr->sender = this->sender;

	buffer->pop();
}

void DirectedEvent::rehydrate(BufferBuilder *buffer) {
	Event::rehydrate(buffer);

	buffer->reserve(sizeof(struct EventHeader));

	struct DirectedEventHeader *hdr = reinterpret_cast<struct DirectedEventHeader *>(buffer->getPointer());

	this->receiver = hdr->receiver;
	this->sender = hdr->sender;

	if (this->child != nullptr) {
		this->child->rehydrate(buffer);
	}

	// no pop
}

Handle& DirectedEvent::getReceiver() {
	return this->receiver;
}

DirectedEvent * DirectedEvent::forReading() {
	DirectedEvent *event = new DirectedEvent();
	event->child = new BufferRemainder();
	return event;
}