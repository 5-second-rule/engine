#pragma once

#include "engine-core.h"
#include "EngineInstance.h"

class COREDLL ServerEngineInstance : public EngineInstance
{
private:
	const float secondsPerTick;

protected:
	virtual void tick(float dt);
	virtual void frame(float dt);

public:
	ServerEngineInstance(World *world, ObjectCtorTable *objectCtors, float minimumFrameTime);

	~ServerEngineInstance();

	virtual void run();
};

