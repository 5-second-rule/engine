#include "UpdateEvent.h"


UpdateEvent::UpdateEvent(Handle handle, ISerializable* child)
	: Event(UpdateEvent::TYPE)
	, handle(handle)
	, child(child)
{}


UpdateEvent::~UpdateEvent() {}

const Handle& UpdateEvent::getHandle() {
	return handle;
}

ISerializable* UpdateEvent::getChild() {
	return child;
}

// ISerializable Methods
void UpdateEvent::reserveSize(IReserve& buffer) {
	handle.reserveSize(buffer);
	child->reserveSize(buffer);
}

void UpdateEvent::fillBuffer(IFill& buffer) {
	handle.fillBuffer(buffer);
	child->fillBuffer(buffer);
}

void UpdateEvent::deserialize(BufferReader& reader) {
	handle.deserialize(reader);
	child->deserialize(reader);
}
