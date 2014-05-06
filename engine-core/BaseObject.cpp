#include <cstring>

#include "BaseObject.h"

BaseObject::BaseObject(int type) : IHasHandle(type) {}

BaseObject::~BaseObject(){}

Handle BaseObject::getHandle(){
	return this->handle;
}

void BaseObject::setHandle(Handle handle){
	this->handle = handle;
}

void BaseObject::reserveSize(IReserve& buffer) {
	handle.reserveSize(buffer);
}

void BaseObject::fillBuffer(IFill& buffer) {
	handle.fillBuffer(buffer);
}

void BaseObject::deserialize(BufferReader& buffer) {
	handle.deserialize(buffer);
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

void BaseObject::enqueue(Event *evt) {
	waitingEvents.push(evt);
}