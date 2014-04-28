#include "World.h"
#include <iostream>

World::World() {
	for (int type = 0; type < 2; type++) {
		this->lastAllocatedIndex[type] = 0;
		this->objectIds[type] = 0;
		this->objects[type].reserve(DEFAULT_OBJECT_ALLOC);
	}
	
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

IHasHandle * World::get(Handle *handle) {
	std::vector<IHasHandle *> *storage = &this->objects[handle->getType()];

	IHasHandle *object = storage->at(handle->index);
	if (object != nullptr && object->getHandle().id == handle->id) {
		return object;
	}

	return nullptr;
}

void World::dehydrate( char *dst, size_t &size, size_t dstSize ) {
	size = 0;

	auto iterator = this->serializable.begin();
	while( iterator != this->serializable.end() ) {
		size_t tmpSize = 0;
		
		//TODO throw exception
		if( size > dstSize )
			return;

		ISerializable *serializable = dynamic_cast<ISerializable *>(this->get( &*iterator ));

		if( serializable != nullptr ) {
			serializable->dehydrate(dst + size, tmpSize, dstSize - size );
		}

		size += tmpSize;
		iterator++;
	}
}

void World::rehydrate( char *data ) {
	/*	Array of bytes contains tagged data, one after another
			------------------------------------------------------------------------
			| index | id | position[3] | velocity[3] | accleration[3] | force[3]
			------------------------------------------------------------------------
	*/


}

void World::update(int dt) {
	auto iterator = this->updatable.begin();
	while (iterator != this->updatable.end()) {
		IUpdatable *updatable = dynamic_cast<IUpdatable *>(this->get(&*iterator));

		if (updatable != nullptr) {
			updatable->update(dt);
		}

		iterator++;
	}
}