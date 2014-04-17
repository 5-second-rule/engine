#pragma once

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"

class COREDLL EngineInstance
{
private:
	ObjectCtorTable *objectCtors;
	World *world;

public:
	EngineInstance(World *world, ObjectCtorTable *objectCtors);
	~EngineInstance();

	// TODO import networking, etc
};

