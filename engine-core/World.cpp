#include "World.h"
#include "IEventReceiver.h"
#include "UpdateEvent.h"

World::World() : updatable(this), serializable(this) {
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

	this->lastAllocatedIndex[handleType] = ++nextIndex;
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

void World::replace( const Handle& handle,  IHasHandle* object) {
	std::vector<IHasHandle *> *storage = &this->objects[handle.getType()];

	if( handle.index >= storage->size() ) {
		throw runtime_error( "Something bad has happened, you are trying to replace an object but it no exist." );
	}

	IHasHandle* oldObject = storage->at(handle.index);
	storage->at( handle.index ) = object;
	delete oldObject;
}

void World::update(float dt) {
	frameCounter = (frameCounter + 1)%1000000000;
	for (int i = 0; i < this->updatable.size(); i++) {
		IUpdatable *updatable = this->updatable.getIndirect(i, false);

		if (updatable != nullptr) {
			updatable->update(dt);
		}
	}
}

void World::broadcastUpdates(CommsProcessor *comms) {
	for (int i = 0; i < this->serializable.size(); i++) {
		IHasHandle *object;
		ISerializable *serializable = this->serializable.getIndirect(i, false, &object);
		BaseObject *bo = dynamic_cast<BaseObject*>(serializable);

		if (bo != nullptr) {
			UpdateEvent* event = new UpdateEvent(object->getHandle(), bo);
			comms->sendEvent(event);
		}
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
