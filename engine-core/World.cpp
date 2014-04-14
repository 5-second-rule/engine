#include "World.h"
#include <iostream>

World::World() {
	this->objectCount = 0;
	this->lastAllocatedSlot = 0;
	memset(this->objectTable, 0, sizeof(this->objectTable));
}

World::~World() {
	// TODO not entirely clear if this is the right place to do destruction of these objects
	for (int i = 0; i < World::MAX_WORLD_OBJECTS; ++i){
		delete objectTable[i];
	}
}

int World::findFreeSlotInHandleTable() {
	int nextIndex = this->lastAllocatedSlot;
	if (this->objectCount > MAX_WORLD_OBJECTS){
		// TODO
	}

	while (objectTable[nextIndex] != nullptr){
		nextIndex = (nextIndex + 1) % World::MAX_WORLD_OBJECTS;
	}

	return this->lastAllocatedSlot = nextIndex;
}

int World::getObjectCount() {
	return objectCount;
}

void World::insert(int index, IHasWorldObject* object) {
	this->objectTable[index] = object;
	this->objectCount++;
}

void World::remove(int index) {
	this->objectTable[index] = nullptr;
	this->objectCount--;
}