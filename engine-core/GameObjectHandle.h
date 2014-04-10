#include "GameObjectId.h"
#include "GameObject.h"
#include "engine-core.h"

#pragma once
class COREDLL GameObjectHandle
{
private:
	int handlerIndex;
	GameObjectId uniqueId;
public:
	GameObjectHandle(GameObject*);
	virtual ~GameObjectHandle();
	GameObject* toObject() const;
};

