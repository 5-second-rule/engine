#include "IHasWorldObject.h"
#include "engine-core.h"

#pragma once
class World
{
private:
	static const int MAX_WORLD_OBJECTS = 10000;
	int objectCount;
	int findFreeSlotInHandleTable();
	int lastAllocatedSlot;
	
	World();
public:
	IHasWorldObject* objectTable[MAX_WORLD_OBJECTS];

	~World();
	int getObjectCount();

	void insert(int index, IHasWorldObject* object);
	void remove(int index);
};
