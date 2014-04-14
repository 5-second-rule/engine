#include "WorldObject.h"
#include "engine-core.h"

#pragma once
class World
{
private:
	static const int MAX_GAME_OBJECTS = 10000;
	int objectCount;
	int findFreeSlotInHandleTable();
	int lastAllocatedSlot;
	
	World();
public:
	WorldObject* objectTable[MAX_GAME_OBJECTS];

	~World();
	int getObjectCount();

	void insert(int index, WorldObject* object);
	void remove(int index);
};
