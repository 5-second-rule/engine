#include "engine-core.h"

#pragma once
class COREDLL WorldObject
{
private:
	int uniqueId;
	int handleIndex;
public:
	WorldObject();
	virtual ~WorldObject();
};

