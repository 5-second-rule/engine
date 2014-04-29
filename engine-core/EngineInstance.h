#pragma once

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"
#include "CommsProcessor.h"
#include "DoubleBufferedQueue.h"

typedef void(*special_event_handler)(BufferBuilder *buffer);

class COREDLL EngineInstance
{
private:
	DoubleBufferedQueue<QueueItem> networkUpdates;
	special_event_handler specialEventHandler;

protected:
	CommsProcessor *comms;
	ObjectCtorTable *objectCtors;
	World *world;
	virtual bool shouldContinueFrames();
	virtual void frame(int dt);
	virtual void dispatchUpdate(QueueItem &item);

	virtual void dispatchObjectUpdate(BufferBuilder *buffer);

	void processNetworkUpdates();

public:
	EngineInstance(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role);

	~EngineInstance();

	virtual void run();
	void sendOutboundEvent(Event *evt);
	void setInboundEventHandler(special_event_handler handler);
};

