#include "UpdateEvent.h"
#include "IHasHandle.h"


UpdateEvent::UpdateEvent(Handle handle, ISerializable* child)
	: Event(UpdateEvent::TYPE)
	, handle(handle)
	, child(child)
{
	if( child != nullptr )
		this->childType = dynamic_cast<IHasHandle*>(child)->getType();
}

UpdateEvent::~UpdateEvent() {}

const Handle& UpdateEvent::getHandle() {
	return handle;
}

ISerializable* UpdateEvent::getChild() {
	return child;
}

void UpdateEvent::setChild( ISerializable* child ) {
	this->child = child;
}

// ISerializable Methods
void UpdateEvent::reserveSize(IReserve& buffer) const {
	Event::reserveSize( buffer );
	buffer.reserve( sizeof(int) );
	handle.reserveSize(buffer);
	child->reserveSize(buffer);
}

void UpdateEvent::fillBuffer(IFill& buffer) const {
	Event::fillBuffer( buffer );
	*reinterpret_cast<int*>(buffer.getPointer()) = this->childType;
	buffer.filled();
	handle.fillBuffer(buffer);
	child->fillBuffer(buffer);
}

void UpdateEvent::deserialize(BufferReader& reader) {
	Event::deserialize( reader );
	this->childType = *reinterpret_cast<const int*>(reader.getPointer());
	reader.finished( sizeof( int ) );
	handle.deserialize(reader);
	if(child != nullptr )
		child->deserialize(reader);
}
