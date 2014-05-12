#include "World.h"

#include <iostream>

#include "IEventReceiver.h"
#include "UpdateEvent.h"

World::World() {
	for (int type = 0; type < 2; type++) {
		this->lastAllocatedIndex[type] = 0;
		this->objectIds[type] = 0;
		this->objects[type].reserve(DEFAULT_OBJECT_ALLOC);
	}
	frameCounter = 0;
	this->updatable.reserve(DEFAULT_OBJECT_ALLOC);
	this->serializable.reserve(DEFAULT_OBJECT_ALLOC);
}

World::~World() {
	for (int i = 0; i < 2; ++i)
		for (std::vector<IHasHandle *>::iterator it = objects[i].begin(); it != objects[i].end(); ++it)
			delete *it;
}

void World::allocateHandle(IHasHandle *object, HandleType handleType) {
	int nextIndex = this->lastAllocatedIndex[handleType];
	// TODO implement better allocator, with wrap around
	//while (objects[handleType].at(nextIndex) != nullptr){
		//nextIndex = (nextIndex + 1);
	//}

	this->lastAllocatedIndex[handleType] = nextIndex++;
	object->setHandle(Handle(nextIndex, objectIds[handleType]++, handleType));
}

void World::insert(IHasHandle *object) {
	Handle handle = object->getHandle();

	std::vector<IHasHandle *> *storage = &this->objects[handle.getType()];

	while ((int)storage->size() <= handle.index) {
		storage->push_back(nullptr);
	}

	*(storage->begin() + handle.index) = object;

	if (dynamic_cast<IUpdatable*>(object) != nullptr) {
		this->updatable.push_back(handle);
	}

	if (dynamic_cast<ISerializable*>(object) != nullptr) {
		this->serializable.push_back(handle);
	}
}

void World::remove(Handle *handle) {
	std::vector<IHasHandle *> *storage = &this->objects[handle->getType()];

	IHasHandle *object = storage->at(handle->index);
	if (object != nullptr && object->getHandle().id == handle->id) {
		storage->at(handle->index) = nullptr;
	}

	// TODO remove from updatable and serializable vectors, in a separate pass
}

IHasHandle * World::get(const Handle& handle) {
	std::vector<IHasHandle *> *storage = &this->objects[handle.getType()];

	if (handle.index >= storage->size()) {
		return nullptr;
	}

	IHasHandle *object = storage->at(handle.index);
	if (object != nullptr && object->getHandle().id == handle.id) {
		return object;
	}

	return nullptr;
}

void World::replace( Handle& handle,  IHasHandle* object) {
	std::vector<IHasHandle *> *storage = &this->objects[handle.getType()];

	if( handle.index >= storage->size() ) {
		throw runtime_error( "Something bad has happened, you are trying to replace an object but it no exist." );
	}

	storage->at( handle.index ) = object;
}

void World::update(float dt) {
	frameCounter = (frameCounter + 1)%1000000000;
	auto iterator = this->updatable.begin();
	while (iterator != this->updatable.end()) {
		IUpdatable *updatable = dynamic_cast<IUpdatable *>(this->get(*iterator));

		if (updatable != nullptr) {
			updatable->update(dt);
		}

		iterator++;
	}
}

void World::broadcastUpdates(CommsProcessor *comms) {
	auto iterator = this->serializable.begin();
	while (iterator != this->serializable.end()) {
		IHasHandle *object = this->get(*iterator);
		ISerializable *serializable = dynamic_cast<ISerializable *>(object);

		if (serializable != nullptr) {
			UpdateEvent* event = new UpdateEvent(object->getHandle(), serializable);
			comms->sendEvent(event);
		}

		iterator++;
	}
}

void World::dispatchEvent(Event *evt, Handle &handle) {
	IHasHandle *obj = this->get(handle);
	IEventReceiver *reciever = dynamic_cast<IEventReceiver *>(obj);

	if (reciever != nullptr) {
		// object responsible for deletion
		reciever->enqueue(evt);
	} else  {
		// this little event is going places; not to an object, but places
		delete evt;
	}
}

bool World::isTick(long int n){
	return frameCounter % n == 0;
}