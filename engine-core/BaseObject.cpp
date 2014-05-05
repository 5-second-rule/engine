#include <cstring>

#include "BaseObject.h"

BaseObject::BaseObject(int objectType){
	this->objectType = objectType;

	this->position[0] = 0;
	this->position[1] = 0;
	this->position[2] = 0;
}

BaseObject::~BaseObject(){

}

Handle BaseObject::getHandle(){
	return Handle();
}

void BaseObject::setHandle(Handle handle){
	this->handle = handle;
}

void BaseObject::reserveSize(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct BaseObjectInfo));
}

void BaseObject::fillBuffer(BufferBuilder *buffer) {
	struct BaseObjectInfo *hdr = reinterpret_cast<struct BaseObjectInfo *>(buffer->getPointer());

	memcpy( hdr->position, position, sizeof( float ) * 3 );
	memcpy( hdr->force, force, sizeof( float ) * 3 );

	buffer->pop();
}

void BaseObject::deserialize(BufferReader& buffer) {
	const struct BaseObjectInfo *hdr = reinterpret_cast<const struct BaseObjectInfo *>(buffer.getPointer());

	memcpy(position, hdr->position, sizeof(float) * 3);
	memcpy(force, hdr->force, sizeof(float) * 3);

	buffer.finished(sizeof(struct BaseObjectInfo));
}

void BaseObject::update(float dt) {
	while (!this->waitingEvents.empty()){
		Event * event = this->waitingEvents.front();
		this->waitingEvents.pop();

		this->handleEvent(event);

		delete event;
	}
}

bool BaseObject::handleEvent(Event *evt) {
	// do nothing by default
	return true;
}

int BaseObject::getType() {
	return this->objectType;
}

void BaseObject::enqueue(Event *evt) {
	waitingEvents.push(evt);
}

void BaseObject::setWorld(World* world){
	m_world = world;
}

World* BaseObject::getWorld(){
	return m_world;
}