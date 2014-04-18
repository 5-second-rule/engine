#pragma once

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"

class COREDLL EngineInstance
{
private:
	ObjectCtorTable *objectCtors;
	World *world;

protected:
	virtual bool shouldContinueFrames();
	virtual void frame();

public:
	EngineInstance(World *world, ObjectCtorTable *objectCtors);
	~EngineInstance();

	virtual void run();

	// TODO import networking, etc
};

