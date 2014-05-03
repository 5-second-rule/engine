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

<<<<<<< HEAD:engine-core/EngineInstance.h
// definitely delete this once player id-ing by guid is working
unsigned int gId;
size_t gIndex;


class COREDLL IEngineInstanceDelegate {
public:
	virtual void HandleAction( ActionEvent* evt ) = 0;
};


class COREDLL EngineInstance {
=======

class COREDLL Engine
{
>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a:engine-core/Engine.h
private:
	DoubleBufferedQueue<QueueItem> networkUpdates;
	special_event_handler specialEventHandler;

	bool running;

protected:
	CommsProcessor *comms;
	ObjectCtorTable *objectCtors;
<<<<<<< HEAD:engine-core/EngineInstance.h
	// put world back here and write an accessor
=======
	World *world;

	const float secondsPerTick;

	virtual bool checkForTick(float dt);
	virtual void tick(float dt);

>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a:engine-core/Engine.h
	virtual bool shouldContinueFrames();
	virtual void frame(float dt) = 0;
	virtual void dispatchUpdate(QueueItem &item);

<<<<<<< HEAD:engine-core/EngineInstance.h
	virtual void dispatchObjectUpdate(BufferReader *buffer);
	virtual void dispatchAction( BufferReader *buffer );
=======
	virtual void updateObject(BufferReader& buffer);
>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a:engine-core/Engine.h

	void processNetworkUpdates();

	IEngineInstanceDelegate* delegate;

public:
	Engine(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role);

<<<<<<< HEAD:engine-core/EngineInstance.h
	~EngineInstance();
	World *world;
=======
	~Engine();
>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a:engine-core/Engine.h

	virtual void run();
	virtual void stop();

	void sendOutboundEvent(Event *evt);
	void setInboundEventHandler(special_event_handler handler);
};
