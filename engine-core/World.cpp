#include "World.h"
#include "IEventReceiver.h"
#include "UpdateEvent.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

World::World() {
	for (int type = 0; type < 2; type++) {
		this->lastAllocatedIndex[type] = -1;
		this->objectIds[type] = 0;
		this->objects[type].reserve(DEFAULT_OBJECT_ALLOC);
	}

	this->updatable = new GCHandleVector<IUpdatable>( this );
	this->serializable = new GCHandleVector<ISerializable>( this );
	this->collidable = new GCHandleVector<ICollidable>( this );

	this->updatable->reserve(DEFAULT_OBJECT_ALLOC);
	this->serializable->reserve(DEFAULT_OBJECT_ALLOC);
	this->collidable->reserve(DEFAULT_OBJECT_ALLOC);
}

World::~World() {
	for( size_t i = 0; i < 2; ++i )
		for( size_t j = 0; j < objects[i].size(); ++j )
			delete this->objects[i][j];

	delete this->updatable;
	delete this->serializable;
	delete this->collidable;
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
		this->updatable->push_back(handle);
	}

	if (dynamic_cast<ISerializable*>(object) != nullptr) {
		this->serializable->push_back(handle);
	}

	if (dynamic_cast<ICollidable*>(object) != nullptr) {
		this->collidable->push_back(handle);
	}
}

void World::remove(Handle *handle) {
	std::vector<IHasHandle *> *storage = &this->objects[handle->getType()];

	IHasHandle *object = storage->at(handle->index);
	if (object != nullptr && object->getHandle().id == handle->id) {
		storage->at(handle->index) = nullptr;
	}

	// TODO remove from updatable, serializable, and collidable vectors, in a separate pass
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

	for (int i = 0; i < this->updatable->size(); i++) {
		IUpdatable *u = this->updatable->getIndirect(i, false);

		if (u != nullptr) {
			u->update(dt);
		}
	}

	//also do collisions

	// naive n^2/2 looping.
	for (int i = 0; i < this->collidable->size(); i++) {
		ICollidable* c1 = this->collidable->getIndirect(i, false);
		for (int j = i + 1; j < this->collidable->size(); j++) {
			ICollidable* c2 = this->collidable->getIndirect(j, false);

			unsigned int p1 = c1->getPriority();
			unsigned int p2 = c2->getPriority();

			Common::Vector4 g1 = c1->getGroupingParameter();
			Common::Vector4 g2 = c2->getGroupingParameter();

			shared_ptr<const Bounds> b1 = c1->getBounds();
			shared_ptr<const Bounds> b2 = c2->getBounds();

			if ((p1 <= p2) ? c1->collidesWith(c2) : c2->collidesWith(c1)) {
				std::cout << "Collision" << std::endl;
				c1->handleCollision(b2, dt);
				c2->handleCollision(b1, dt);
			}
		}
	}

}

void World::broadcastUpdates(CommsProcessor *comms) {
	for (int i = 0; i < this->serializable->size(); i++) {
		IHasHandle *object;
		ISerializable *serializable = this->serializable->getIndirect(i, false, &object);
		BaseObject *bo = dynamic_cast<BaseObject*>(serializable);

		if (bo != nullptr && !(bo->getHandle().isLocal())) {
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
