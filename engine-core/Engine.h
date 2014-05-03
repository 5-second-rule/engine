#pragma once
#include <chrono>

#include "ObjectCtorTable.h"
#include "World.h"
#include "engine-core.h"
#include "CommsProcessor.h"
#include "DoubleBufferedQueue.h"
#include "ActionEvent.h"

typedef void(*special_event_handler)(BufferReader& buffer);
typedef std::chrono::duration<float, ratio<1, 1>> float_seconds;

// definitely delete this once player id-ing by guid is working
extern unsigned int gId;
extern size_t gIndex;


class COREDLL IEngineInstanceDelegate {
public:
	virtual void HandleAction( ActionEvent* evt ) = 0;
	virtual ActionEvent* MakeActionEvent( int actionType, unsigned int playerGuid, size_t index, const char* data ) = 0;
};


class COREDLL Engine {
private:
	DoubleBufferedQueue<QueueItem> networkUpdates;
	special_event_handler specialEventHandler;

	bool running;

protected:
	CommsProcessor *comms;
	ObjectCtorTable *objectCtors;
	// put world back here and write an accessor
	const float secondsPerTick;

	virtual bool checkForTick(float dt);
	virtual void tick(float dt);
	virtual bool shouldContinueFrames();
	virtual void frame(float dt) = 0;
	virtual void dispatchUpdate(QueueItem &item);
	virtual void dispatchAction( BufferReader *buffer );
	virtual void updateObject(BufferReader& buffer);

	void processNetworkUpdates();

	IEngineInstanceDelegate* delegate;

public:
	Engine(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role);

	World *world;
	~Engine();

	virtual void run();
	virtual void stop();

	void sendOutboundEvent(Event *evt);
	void setInboundEventHandler(special_event_handler handler);
};
