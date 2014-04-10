#include "GameObject.h"
#include "GameObjectId.h"
#include "World.h"

GameObject::GameObject()
{
	this->uniqueId = GameObjectId::assignUniqueObjectId();
	this->handleIndex = World::findFreeSlotInHandleTable();
	World::objectTable[handleIndex] = this;
	World::objectCounter++;
}


GameObject::~GameObject()
{
	World::objectTable[handleIndex] = nullptr;
	World::objectCounter--;
}


