#pragma once

#include "engine-core.h"
#include "Engine.h"

class COREDLL ServerEngine : public Engine
{
private:
	const float secondsPerTick;

protected:
	virtual void tick(float dt);
	virtual void frame(float dt);

public:
	ServerEngine(World *world, ObjectCtorTable *objectCtors, float minimumFrameTime);

	~ServerEngine();

	virtual void run();
};

