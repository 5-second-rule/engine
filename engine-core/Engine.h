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

#include <map>

typedef void(*special_event_handler)(BufferReader& buffer);
typedef std::chrono::duration<float, ratio<1, 1>> float_seconds;

class COREDLL Engine {
private:
	DoubleBufferedQueue<QueueItem> networkUpdates;
	special_event_handler specialEventHandler;

	bool running;
	bool waitingForRegistration;
	RegistrationRequestHeader waitingRegistration;

protected:
	CommsProcessor *comms;
	ConstructorTable<IHasHandle> *objectCtors;
	EventFactory *eventCtors;

	std::map<unsigned int, Handle> playerMap;
	std::vector<unsigned int> localPlayers;

	World *world;

	const float secondsPerTick;

public:
	Engine(
		World *world,
		ConstructorTable<IHasHandle> *objectCtors,
		ConstructorTable<ActionEvent>* eventCtors,
		CommsProcessorRole role
	);

	virtual ~Engine();

	virtual void run();
	virtual void stop();

	World* getWorld();

	void sendOutboundEvent(Event *evt);
	void setInboundEventHandler(special_event_handler handler);

	void registerPlayer(bool wait);
	unsigned int getLocalPlayerGuid(unsigned int playerIndex);

protected:
	virtual bool checkForTick(float dt);
	virtual void tick(float dt);

	virtual bool shouldContinueFrames();
	virtual void frame(float dt) = 0;

	virtual void dispatchUpdate(QueueItem &item);
	virtual void dispatchAction(ActionEvent *buffer);

	virtual void updateObject(UpdateEvent* evt, BufferReader& reader);

	virtual void handleRegistrationRequest(BufferReader& buffer);
	virtual void handleRegistrationResponse(BufferReader& buffer);

	void processNetworkUpdates();
};
