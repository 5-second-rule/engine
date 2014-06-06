#include "BaseObject.h"

BaseObject::BaseObject(int type) : IHasHandle(type) {}

BaseObject::~BaseObject(){}

Handle BaseObject::getHandle(){
	return this->handle;
}

void BaseObject::setHandle(Handle handle){
	this->handle = handle;
}

void BaseObject::setGC(){
	this->handle.gc = true;
}

void BaseObject::reserveSize(IReserve& buffer) const {
	buffer.reserve(sizeof(int));
	handle.reserveSize(buffer);
}

void BaseObject::fillBuffer(IFill& buffer) const {
	*reinterpret_cast<int*>(buffer.getPointer()) = IHasHandle::getType();
	buffer.filled();

	handle.fillBuffer(buffer);
}

void BaseObject::deserialize(BufferReader& buffer) {
	this->setType(*reinterpret_cast<const int*>(buffer.getPointer()));
	buffer.finished(sizeof(int));
	handle.deserialize(buffer);
}

int BaseObject::getType(BufferReader& buffer) {
	return *reinterpret_cast<const int*>(buffer.getPointer());
}

void BaseObject::update(float dt) {
	while (!this->waitingEvents.empty()){
		Event * event = this->waitingEvents.front();
		this->waitingEvents.pop();

		this->handleEvent(event);

		delete event;
	}
}

void BaseObject::enqueue(Event *evt) {
	waitingEvents.push(evt);
}

std::string BaseObject::toString() const {
	return this->handle.toString();
}