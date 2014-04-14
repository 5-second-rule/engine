#include "IHasWorldObject.h"
#include "engine-core.h"

#pragma once
class COREDLL World
{
private:
	static const int MAX_WORLD_OBJECTS = 10000;
	int objectCount;
	int findFreeSlotInHandleTable();
	int lastAllocatedSlot;
	
	
public:
	IHasWorldObject* objectTable[MAX_WORLD_OBJECTS];

	World();
	~World();

	int getObjectCount();

	void insert(int index, IHasWorldObject* object);
	void remove(int index);
};
