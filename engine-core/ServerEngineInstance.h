#pragma once

#include "engine-core.h"
#include "EngineInstance.h"

class COREDLL ServerEngineInstance : public EngineInstance
{
private:
	const float secondsPerTick;

protected:
	virtual bool checkForTick(float dt) = 0;
	virtual void tick(float dt) = 0;

	virtual void frame(float dt) = 0;

public:
	ServerEngineInstance(World *world, ObjectCtorTable *objectCtors, float minimumFrameTime);

	~ServerEngineInstance();

	virtual void run();
};

