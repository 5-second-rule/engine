#pragma once

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"
#include "CommsProcessor.h"

#include <queue>

class COREDLL EngineInstance
{
private:
	queue<QueueItem> networkUpdates;

protected:
	CommsProcessor *comms;
	ObjectCtorTable *objectCtors;
	World *world;
	virtual bool shouldContinueFrames();
	virtual void frame(int dt);
	virtual void dispatchUpdate(QueueItem &item);

	void processNetworkUpdates();

public:
	EngineInstance(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role);

	~EngineInstance();

	virtual void run();
};

