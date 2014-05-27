#pragma once
#include <chrono>

#include "ConstructorTable.h"
#include "World.h"
#include "engine-core.h"
#include "CommsProcessor.h"
#include "DoubleBufferedQueue.h"
#include "ActionEvent.h"
#include "EventFactory.h"
#include "UpdateEvent.h"
#include "SoundEvent.h"
#include "RegistrationEvent.h"
#include "BaseObject.h"

#include <map>

typedef std::chrono::duration<float, std::ratio<1, 1>> float_seconds;

class World;
enum class CommsProcessorRole;

class COREDLL Engine {
	friend class CommsProcessor;
private:
	DoubleBufferedQueue<Event*> networkUpdates;

	bool running;
	bool waitingForRegistration;
	RegistrationEvent waitingRegistration;

protected:
	CommsProcessor *comms;
	ConstructorTable<BaseObject> *objectCtors;
	EventFactory *eventCtors;

	std::map<unsigned int, Handle> playerMap;
	std::vector<unsigned int> localPlayers;

	World *world;

	const float secondsPerTick;

public:
	Engine(
		World *world,
		ConstructorTable<BaseObject> *objectCtors,
		ConstructorTable<ActionEvent>* eventCtors,
		CommsProcessorRole role
	);

	virtual ~Engine();

	void sendEvent( Event* evt );

	bool isRunning();
	virtual void run();
	virtual void stop();

	World* getWorld();

	void registerPlayer(bool wait);
	unsigned int getLocalPlayerGuid(unsigned int playerIndex);

protected:
	virtual bool checkForTick(float dt);
	virtual void tick(float dt);

	virtual bool shouldContinueFrames();
	virtual void frame(float dt) = 0;

	virtual void dispatchUpdate( Event* event );
	virtual void dispatchAction( ActionEvent *evt );
	virtual void dispatchSound( SoundEvent *evt );

	virtual void updateObject( UpdateEvent* evt );

	virtual void handleRegistrationRequest( RegistrationEvent* event );
	virtual void handleRegistrationResponse( RegistrationEvent* event );

	void processNetworkUpdates();
};
