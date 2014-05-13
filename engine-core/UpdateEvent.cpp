#include "UpdateEvent.h"
#include "IHasHandle.h"


UpdateEvent::UpdateEvent(Handle handle, BaseObject* child)
	: Event(UpdateEvent::TYPE)
	, handle(handle)
	, child(child)
{}

UpdateEvent::~UpdateEvent() {}

const Handle& UpdateEvent::getHandle() {
	return handle;
}

BaseObject* UpdateEvent::getChild() {
	return child;
}

void UpdateEvent::setChild( BaseObject* child ) {
	this->child = child;
}

// ISerializable Methods
void UpdateEvent::reserveSize(IReserve& buffer) const {
	Event::reserveSize( buffer );
	handle.reserveSize(buffer);
	if(child) child->reserveSize(buffer);
}

void UpdateEvent::fillBuffer(IFill& buffer) const {
	Event::fillBuffer( buffer );
	handle.fillBuffer(buffer);
	if(child) child->fillBuffer(buffer);
}

void UpdateEvent::deserialize(BufferReader& reader) {
	Event::deserialize( reader );
	handle.deserialize( reader );
	// child has to be deserialized elsewhere
}