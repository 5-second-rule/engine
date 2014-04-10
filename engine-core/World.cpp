#include "World.h"
#include <Windows.h>
#include <iostream>

bool World::instanceFlag = false;
int World::objectCounter = 0;
GameObject* World::objectTable[World::MAX_GAME_OBJECTS] = { nullptr };
World* World::world = World::getInstance();

World::World()
{
}


World::~World()
{
	delete world;

	for (int i = 0; i < World::MAX_GAME_OBJECTS; ++i){
		delete objectTable[i];
	}
}

World* World::getInstance(){
	if (!World::instanceFlag){
		World::world = new World();
		World::instanceFlag = true;
	}
	return World::world;
}

int World::findFreeSlotInHandleTable(){
	static int nextIndex = 0;

	if (World::objectCounter > MAX_GAME_OBJECTS){
		// todo 
	}

	while (objectTable[nextIndex] != nullptr){
		nextIndex = (nextIndex + 1) % World::MAX_GAME_OBJECTS;
	}

	return nextIndex;
}