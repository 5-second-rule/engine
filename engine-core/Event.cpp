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

<<<<<<< HEAD
void Event::deserialize(BufferReader *buffer) {
	buffer->finished(sizeof(struct EventHeader));
=======
void Event::deserialize(BufferReader& buffer) {
	const struct EventHeader *hdr = reinterpret_cast<const struct EventHeader *>(buffer.getPointer());
	this->type = (EventType)hdr->type;

	buffer.finished(sizeof(struct EventHeader));
>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a
}