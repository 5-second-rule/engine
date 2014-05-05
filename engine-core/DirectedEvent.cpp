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

void DirectedEvent::reserveSize(IReserve& buffer) {
	Event::reserveSize(buffer);

	buffer.reserve(sizeof(struct EventHeader));

	if (this->child != nullptr) {
		this->child->reserveSize(buffer);
	}
}

void DirectedEvent::fillBuffer(IFill& buffer) {
	Event::fillBuffer(buffer);

	struct DirectedEventHeader *hdr = reinterpret_cast<struct DirectedEventHeader *>(buffer.getPointer());
	hdr->receiver = this->receiver;
	hdr->sender = this->sender;

	buffer.filled();

	if (this->child != nullptr) {
		this->child->fillBuffer(buffer);
	}
}

void DirectedEvent::deserialize(BufferReader& buffer) {
	Event::deserialize(buffer);

	const struct DirectedEventHeader *hdr = reinterpret_cast<const struct DirectedEventHeader *>(buffer.getPointer());
	this->receiver = hdr->receiver;
	this->sender = hdr->sender;

	buffer.finished(sizeof(struct EventHeader));

	if (this->child != nullptr) {
		this->child->deserialize(buffer);
	}
}

Handle& DirectedEvent::getReceiver() {
	return this->receiver;
}

DirectedEvent * DirectedEvent::forReading() {
	DirectedEvent *event = new DirectedEvent();
	event->child = new BufferRemainder();
	return event;
}