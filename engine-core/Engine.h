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
#include "RegistrationEvent.h"
#include "BaseObject.h"

#include <map>

typedef void(*special_event_handler)(BufferReader& buffer);
typedef std::chrono::duration<float, std::ratio<1, 1>> float_seconds;

class World;
enum class CommsProcessorRole;

class COREDLL Engine {
	friend class CommsProcessor;
private:
	DoubleBufferedQueue<Event*> networkUpdates;
	special_event_handler specialEventHandler;

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

	virtual void run();
	virtual void stop();

	World* getWorld();

	void setInboundEventHandler(special_event_handler handler);

	void registerPlayer(bool wait);
	unsigned int getLocalPlayerGuid(unsigned int playerIndex);

protected:
	virtual bool checkForTick(float dt);
	virtual void tick(float dt);

	virtual bool shouldContinueFrames();
	virtual void frame(float dt) = 0;

	virtual void dispatchUpdate( Event* event );
	virtual void dispatchAction(ActionEvent *buffer);

	virtual void updateObject(UpdateEvent* evt);

	virtual void handleRegistrationRequest( RegistrationEvent* event );
	virtual void handleRegistrationResponse( RegistrationEvent* event );

	void processNetworkUpdates();
};
