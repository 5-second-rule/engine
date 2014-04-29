#include <cstring>

#include "BaseObject.h"

BaseObject::BaseObject(int objectType){
	this->objectType = objectType;
}

BaseObject::~BaseObject(){

}

Handle BaseObject::getHandle(){
	return Handle();
}

void BaseObject::setHandle(Handle handle){
	this->handle = handle;
}

void BaseObject::dehydrate(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct BaseObjectInfo));
	buffer->allocate();

	struct BaseObjectInfo *hdr = reinterpret_cast<struct BaseObjectInfo *>(buffer->getPointer());

	memcpy( hdr->position, position, sizeof( float ) * 3 );
	memcpy( hdr->force, force, sizeof( float ) * 3 );

	buffer->pop();
}

void BaseObject::rehydrate(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct BaseObjectInfo));

	struct BaseObjectInfo *hdr = reinterpret_cast<struct BaseObjectInfo *>(buffer->getPointer());

	memcpy(hdr->position, position, sizeof(float) * 3);
	memcpy(hdr->force, force, sizeof(float) * 3);

	buffer->pop();
}

void BaseObject::update(int dt) {
}

int BaseObject::getType() {
	return this->objectType;
}

void BaseObject::onEvent(DirectedEvent *evt) {
	waitingEvents.push(evt);
}