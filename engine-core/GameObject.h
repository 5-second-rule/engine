#include "GameObjectId.h"
#include "engine-core.h"

#pragma once
class COREDLL GameObject
{
private:
	friend class GameObjectHandle;

	GameObjectId uniqueId;
	int handleIndex;

public:
	GameObject();
	virtual ~GameObject();
};

