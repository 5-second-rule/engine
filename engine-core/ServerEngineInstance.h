#pragma once

#include "engine-core.h"
#include "EngineInstance.h"

class COREDLL ServerEngineInstance : public EngineInstance
{
private:
	bool running;
	int minimumFrameTime;

protected:
	virtual bool shouldContinueFrames();
	virtual void frame(int dt);

public:
	ServerEngineInstance(
		World *world,
		ObjectCtorTable *objectCtors,
		int minimumFrameTime);

	~ServerEngineInstance();

	virtual void run();
	void stop();
};

