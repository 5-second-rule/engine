#pragma once
#include "engine-core.h"

#include "Engine.h"

#include "CommandLine.h"

class COREDLL ServerEngine : public Engine
{
private:
	const float secondsPerTick;
	CommandLine commandLine;

protected:

	virtual void tick(float dt);
	virtual void frame(float dt);

public:
	ServerEngine(
		World *world,
		ConstructorTable<BaseObject> *objectCtors,
		ConstructorTable<ActionEvent>*,
		float minimumFrameTime);

	~ServerEngine();

	virtual void run();
};

