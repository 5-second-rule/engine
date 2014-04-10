#include "GameObjectId.h"
#include "GameObject.h"

#pragma once
class GameObjectHandle
{
private:
	int handlerIndex;
	GameObjectId uniqueId;
public:
	GameObjectHandle(GameObject*);
	virtual ~GameObjectHandle();
	GameObject* toObject() const;
};

