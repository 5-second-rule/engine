#include "GameObject.h"
#include "GameObjectHandle.h"
#include "engine-core.h"

#pragma once
class COREDLL World
{
	friend class GameObject;
private:
	static const int MAX_GAME_OBJECTS = 10000;
	static bool instanceFlag;
	static World* world;
	static int objectCounter;
	static int findFreeSlotInHandleTable();
	
	World();
public:
	static GameObject* objectTable[MAX_GAME_OBJECTS];

	~World();
	static World* getInstance();
	static int getObjectCounter();
};
