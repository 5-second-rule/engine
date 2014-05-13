#include "Event.h"

Event::~Event() {
}

Event::Event(EventType type) : type(type) {}

void Event::reserveSize(IReserve& buffer) const {
	buffer.reserve(sizeof(struct EventHeader));
}

void Event::fillBuffer(IFill& buffer) const {
	struct EventHeader *hdr = reinterpret_cast<struct EventHeader *>(buffer.getPointer());

	hdr->type = static_cast<int>(this->type);

	buffer.filled();
}

void Event::deserialize(BufferReader& buffer) {
	this->type = Event::getType(buffer);
	buffer.finished(sizeof(struct EventHeader));
}

EventType Event::getType(BufferReader& reader) {
	const struct EventHeader *hdr = reinterpret_cast<const struct EventHeader *>(reader.getPointer());
	return static_cast<EventType>(hdr->type);
}

EventType Event::getType() {
	return this->type;
}

