#include "GameObjectId.h"

#pragma once
class GameObject
{
private:
	friend class GameObjectHandle;

	GameObjectId uniqueId;
	int handleIndex;

public:
	GameObject();
	virtual ~GameObject();
};

