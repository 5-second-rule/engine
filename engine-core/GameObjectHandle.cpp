#include "GameObjectHandle.h"
#include "GameObject.h"
#include "World.h"

GameObjectHandle::GameObjectHandle(GameObject* o)
{
	handlerIndex = o->handleIndex;
	uniqueId = o->uniqueId;
}


GameObjectHandle::~GameObjectHandle()
{
}

GameObject* GameObjectHandle::toObject() const {
	GameObject* tmp = World::objectTable[GameObjectHandle::handlerIndex];

	if ((tmp != nullptr) && (tmp->uniqueId == this->uniqueId))
		return tmp;
	else
		return nullptr;
}
