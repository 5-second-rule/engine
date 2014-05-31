#include "Engine.h"
#include <iostream>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

const static int TICKS_PER_SEC = 25;

using namespace std::chrono;

Engine::Engine(
	World *world,
	ConstructorTable<BaseObject> *objectCtors,
	ConstructorTable<ActionEvent> *eventCtors,
	CommsProcessorRole role
)
	: secondsPerTick(1.0f / (float)TICKS_PER_SEC)
	, world(world)
	, objectCtors(objectCtors)
	, eventCtors(new EventFactory(eventCtors))
{
	this->debugLevel = 0;
	this->running = false;
	// Set up network
	this->comms = new CommsProcessor( role, this );
	this->comms->setHandoffQ( &networkUpdates );
	waitingForRegistration = false;

	srand((unsigned int)time(NULL));
}

Engine::~Engine() {
	// clean out networkUpdates
	while( !this->networkUpdates.readEmpty() ) {
		Event* event = this->networkUpdates.pop();
		if( event->getType() == EventType::UPDATE ) {
			delete Event::cast<UpdateEvent>( event )->getChild();
		}
		delete event;
	}
	this->networkUpdates.swap();
	while( !this->networkUpdates.readEmpty() ) {
		Event* event = this->networkUpdates.pop();
		if( event->getType() == EventType::UPDATE ) {
			delete Event::cast<UpdateEvent>( event )->getChild();
		}
		delete event;
	}

	delete this->world;
	delete this->eventCtors;
	delete this->objectCtors;
}

void Engine::sendEvent( Event* evt ) {
	this->comms->sendEvent( evt );
}

void Engine::run() {
	steady_clock::time_point lastTickTime = steady_clock::now();

	this->running = true;

	while (this->shouldContinueFrames()) {
		steady_clock::time_point start = steady_clock::now();
		float dt = (float)duration_cast<float_seconds>(start - lastTickTime).count();

		if (this->checkForTick(dt)) {
			this->tick(dt);

			lastTickTime = start;
			dt = 0.0f;
		}

		this->frame(dt);		
	}
}

bool Engine::isRunning() {
	return this->running;
}

void Engine::stop() {
	running = false;
	delete this->comms;
}

World* Engine::getWorld() {
	return this->world;
}

bool Engine::shouldContinueFrames() {
	return running;
}

bool Engine::checkForTick(float dt) {
	return dt >= secondsPerTick;
}

void Engine::tick(float dt) {
	this->world->update(dt);
}

void Engine::processNetworkUpdates() {
	// bring new updates forward
	this->networkUpdates.swap();

	while (!this->networkUpdates.readEmpty()) {
		Event* evt = this->networkUpdates.pop();
		dispatchUpdate(evt);
	}
}

void Engine::dispatchUpdate(Event* event) {
	if( event->getType() == EventType::REGISTRATION ) {
		RegistrationEvent* regEvent = Event::cast<RegistrationEvent>( event );
		switch( regEvent->regType ) {
			case RegistrationType::REQUEST:
				if( this->debugLevel > 0 ) std::cout << "player registration inbound" << std::endl;
				this->handleRegistrationRequest( regEvent );
				break;
			case RegistrationType::RESPONSE:
				this->handleRegistrationResponse( regEvent );
				break;
		}
	} else if (!waitingForRegistration) {
		switch (event->getType()) {
		case EventType::UPDATE:
			this->updateObject(Event::cast<UpdateEvent>(event));
			break;
		case EventType::ACTION:
			if( this->debugLevel > 0 ) std::cout << "action!" << std::endl;
			this->dispatchAction(Event::cast<ActionEvent>(event));
			break;
		case EventType::SOUND:
			this->dispatchSound( Event::cast<SoundEvent>( event ) );
			break;
		default:
			if( this->debugLevel > 0 ) std::cout << "Urgh!" << std::endl;
			break;
		}
	} else {
		if( event->getType() == EventType::UPDATE ) {
			delete Event::cast<UpdateEvent>( event )->getChild();
		}
		delete event;
	}
}

void Engine::handleRegistrationRequest(RegistrationEvent* event) {
	static int modNum = 1;

	auto place = this->playerMap.find(event->playerGuid);
	Response response = Response::FAIL;
	Handle resultObjectHandle = Handle();
	if (place == this->playerMap.end()) {
		response = Response::OK;
		// spot is available, yay!

		// HACK make first model for now
		BaseObject * obj = this->objectCtors->invoke( modNum );
		world->allocateHandle(obj, HandleType::GLOBAL);
		world->insert(obj);

		modNum++;

		this->playerMap[event->playerGuid] = obj->getHandle();
		resultObjectHandle = obj->getHandle();

		if( this->debugLevel > 0 ) std::cout << "=> player registered" << std::endl;
	} else {
		if( this->debugLevel > 0 ) std::cout << "=> player NOT registered #fail" << std::endl;
	}

	RegistrationEvent respEvent;
	respEvent.regType = RegistrationType::RESPONSE;
	respEvent.playerGuid = event->playerGuid;
	respEvent.response = response;
	respEvent.responseTag = event->responseTag;
	respEvent.objectHandle = resultObjectHandle;

	delete event;
	comms->sendEvent(&respEvent);
}

// HACK not really safe but good enough for now
// what if server response lost? Object created on server but client doesn't know
void Engine::handleRegistrationResponse( RegistrationEvent* event ) {

	if (event->responseTag == this->waitingRegistration.responseTag
		&& event->response == Response::OK) { 		// matches
		this->waitingForRegistration = false;
		this->localPlayers.push_back(this->waitingRegistration.playerGuid);
		this->playerMap[event->playerGuid] = event->objectHandle;
	}

	delete event;
}

// HACK not really safe either, no prevention of double call
void Engine::registerPlayer(bool wait) {
	this->waitingForRegistration = wait;

	this->waitingRegistration.playerGuid = rand();
	this->waitingRegistration.responseTag = rand();
	this->waitingRegistration.regType = RegistrationType::REQUEST;
	this->waitingRegistration.playerGuid = this->waitingRegistration.playerGuid;

	comms->sendEvent( &this->waitingRegistration );
}

unsigned int Engine::getLocalPlayerGuid(unsigned int playerIndex) {
	if (playerIndex < 0 || playerIndex >= this->localPlayers.size()) {
		// TODO do something
		return 0;
	}

	return this->localPlayers[playerIndex];
}

void Engine::updateObject(UpdateEvent* evt) {

	if (this->world->get(evt->getHandle()) == nullptr) {
		world->insert(evt->getChild());
	} else {
		world->replace( evt->getHandle(), evt->getChild() );
	}

	delete evt;
}

void Engine::dispatchAction( ActionEvent *evt ) {

	auto playerHandle = this->playerMap.find(evt->getPlayerGuid());

	if (playerHandle != this->playerMap.end()) {
		this->world->dispatchEvent(evt, playerHandle->second);
	} else {
		std::cout << "WARN: unknown player attempted an action" << std::endl;
	}
}

void Engine::dispatchSound( SoundEvent *evt ) {
	// delete the event as there is nothing left to do on
	// non-renderable engines
	delete evt;
}