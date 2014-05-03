#include "Event.h"

Event::~Event() {
}

Event::Event() {
}

void Event::reserveSize(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct EventHeader));
}

void Event::fillBuffer(BufferBuilder *buffer) {
	buffer->pop();
}


void Event::deserialize(BufferReader& buffer) {
	const struct EventHeader *hdr = reinterpret_cast<const struct EventHeader *>(buffer.getPointer());
	buffer.finished(sizeof(struct EventHeader));
}