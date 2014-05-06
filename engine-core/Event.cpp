#include "Event.h"

Event::~Event() {
}

Event::Event() {
}

void Event::reserveSize(IReserve& buffer) {
	buffer.reserve(sizeof(struct EventHeader));
}

void Event::fillBuffer(IFill& buffer) {
	struct EventHeader *hdr = reinterpret_cast<struct EventHeader *>(buffer.getPointer());

	hdr->type = this->type;

	buffer.filled();
}

void Event::deserialize(BufferReader& buffer) {
	const struct EventHeader *hdr = reinterpret_cast<const struct EventHeader *>(buffer.getPointer());
	this->type = hdr->type;
	buffer.finished(sizeof(struct EventHeader));
}
