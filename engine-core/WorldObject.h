#include "engine-core.h"

#pragma once
class WorldObject
{
private:
	int uniqueId;
	int handleIndex;
public:
	WorldObject();
	virtual ~WorldObject();
};

