#pragma once

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"
#include "CommsProcessor.h"
#include "DoubleBufferedQueue.h"
#include "ActionEvent.h"

typedef void(*special_event_handler)(BufferReader *buffer);

// definitely delete this once player id-ing by guid is working
unsigned int gId;
size_t gIndex;


class COREDLL IEngineInstanceDelegate {
public:
	virtual void HandleAction( ActionEvent* evt ) = 0;
};


class COREDLL EngineInstance {
private:
	DoubleBufferedQueue<QueueItem> networkUpdates;
	special_event_handler specialEventHandler;

protected:
	CommsProcessor *comms;
	ObjectCtorTable *objectCtors;
	// put world back here and write an accessor
	virtual bool shouldContinueFrames();
	virtual void frame(int dt);
	virtual void dispatchUpdate(QueueItem &item);

	virtual void dispatchObjectUpdate(BufferReader *buffer);
	virtual void dispatchAction( BufferReader *buffer );

	void processNetworkUpdates();

	IEngineInstanceDelegate* delegate;

public:
	EngineInstance(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role);

	~EngineInstance();
	World *world;

	virtual void run();
	void sendOutboundEvent(Event *evt);
	void setInboundEventHandler(special_event_handler handler);
};