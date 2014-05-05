#include <cstring>

#include "BaseObject.h"

BaseObject::BaseObject(){
	//TODO
}

BaseObject::~BaseObject(){

}

Handle BaseObject::getHandle(){
	return this->handle;
}

void BaseObject::setHandle(Handle handle){
	this->handle = handle;
}

void BaseObject::reserveSize(IReserve& buffer) {
	buffer.reserve(sizeof(struct BaseObjectInfo));
}

void BaseObject::fillBuffer(IFill& buffer) {
	struct BaseObjectInfo *hdr = reinterpret_cast<struct BaseObjectInfo *>(buffer.getPointer());

	memcpy( hdr->position, position, sizeof( float ) * 3 );
	memcpy( hdr->force, force, sizeof( float ) * 3 );

	buffer.filled();
}

void BaseObject::deserialize(BufferReader& buffer) {
	const struct BaseObjectInfo *hdr = reinterpret_cast<const struct BaseObjectInfo *>(buffer.getPointer());

	memcpy(position, hdr->position, sizeof(float) * 3);
	memcpy(force, hdr->force, sizeof(float) * 3);

	buffer.finished(sizeof(struct BaseObjectInfo));
}

void BaseObject::update(float dt) {
}

int BaseObject::getType() {
	return this->objectType;
}

void BaseObject::onEvent(DirectedEvent *evt) {
	waitingEvents.push(evt);
}