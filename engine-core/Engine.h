#pragma once
#include <chrono>

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"
#include "CommsProcessor.h"
#include "DoubleBufferedQueue.h"

typedef void(*special_event_handler)(BufferReader& buffer);
typedef std::chrono::duration<float, ratio<1, 1>> float_seconds;


class COREDLL Engine
{
private:
	DoubleBufferedQueue<QueueItem> networkUpdates;
	special_event_handler specialEventHandler;

	bool running;

protected:
	CommsProcessor *comms;
	ObjectCtorTable *objectCtors;
	World *world;

	const float secondsPerTick;

	virtual bool checkForTick(float dt);
	virtual void tick(float dt);

	virtual bool shouldContinueFrames();
	virtual void frame(float dt) = 0;
	virtual void dispatchUpdate(QueueItem &item);

	virtual void updateObject(BufferReader& buffer);

	void processNetworkUpdates();

public:
	Engine(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role);

	~Engine();

	virtual void run();
	virtual void stop();

	void sendOutboundEvent(Event *evt);
	void setInboundEventHandler(special_event_handler handler);
};

