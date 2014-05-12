#include "Engine.h"

#include <iostream>


#define TICKS_PER_SEC 25

using namespace std::chrono;

Engine::Engine(
	World *world,
	ConstructorTable<IHasHandle> *objectCtors,
	ConstructorTable<ActionEvent> *eventCtors,
	CommsProcessorRole role
)
	: secondsPerTick(1.0f / (float)TICKS_PER_SEC)
	, world(world)
	, objectCtors(objectCtors)
	, eventCtors(new EventFactory(eventCtors))
{
	// Set up network
	this->comms = new CommsProcessor(role, this);
	this->comms->setHandoffQ(&networkUpdates);

	waitingForRegistration = false;

	srand((unsigned int)time(NULL));
}

Engine::~Engine() {
	delete this->comms;
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

void Engine::stop() {
	running = false;
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
		RegistionEvent* regEvent = static_cast<RegistionEvent*>(event);
		switch( regEvent->regType ) {
			case RegistrationType::REQUEST:
				if( _DEBUG ) std::cout << "player registration inbound" << std::endl;
				this->handleRegistrationRequest( regEvent );
				break;
			case RegistrationType::RESPONSE:
				this->handleRegistrationResponse( regEvent );
				break;
		}
	}

	if (!waitingForRegistration) {
		switch (event->getType()) {
		case EventType::UPDATE:
			this->updateObject(Event::cast<UpdateEvent>(event));
			break;
		case EventType::ACTION:
			if (_DEBUG) std::cout << "action!" << std::endl;
			this->dispatchAction(Event::cast<ActionEvent>(event));
			break;
		default:
			//TODO log bad event
			break;
		}
	}
}

void Engine::handleRegistrationRequest(RegistionEvent* event) {

	auto place = this->playerMap.find(event->playerGuid);
	Response response = Response::FAIL;
	if (place == this->playerMap.end()) {
		response = Response::OK;
		// spot is available, yay!

		// HACK make first model for now
		IHasHandle * obj = this->objectCtors->invoke(0,nullptr);
		world->allocateHandle(obj, HandleType::GLOBAL);
		world->insert(obj);

		this->playerMap[event->playerGuid] = obj->getHandle();

		if (_DEBUG) std::cout << "=> player registered" << std::endl;
	} else {
		if (_DEBUG) std::cout << "=> player NOT registered #fail" << std::endl;
	}

	RegistionEvent respEvent;
	respEvent.regType = RegistrationType::RESPONSE;
	respEvent.playerGuid = event->playerGuid;
	respEvent.response = response;
	respEvent.responseTag = event->responseTag;

	comms->sendEvent(&respEvent);
}

// HACK not really safe but good enough for now
// what if server response lost? Object created on server but client doesn't know
void Engine::handleRegistrationResponse( RegistionEvent* event ) {

	if (event->responseTag == this->waitingRegistration.responseTag
		&& event->response == Response::OK) { 		// matches
		this->waitingForRegistration = false;
		this->localPlayers.push_back(this->waitingRegistration.playerGuid);
	}
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

	// get or create the object
	IHasHandle *object = this->world->get(evt->getHandle());
	IHasHandle *newObj = dynamic_cast<IHasHandle*>(evt->getChild());

	if( newObj == nullptr ) {
		throw runtime_error( "Something went wrong, your cast has failed you" );
	}

	if (object == nullptr) {
		UpdateArgs args;
		args.handle = Handle();
		args.child = nullptr;
		object = this->objectCtors->invoke(newObj->getType(), &args);
		world->insert(newObj);
	} else {
		world->replace( object->getHandle(), newObj );
	}


}

void Engine::dispatchAction( ActionEvent *evt ) {

	auto playerHandle = this->playerMap.find(evt->getPlayerGuid());

	if (playerHandle != this->playerMap.end()) {
		this->world->dispatchEvent(evt, playerHandle->second);
	} else {
		std::cout << "WARN: unknown player attempted an action" << std::endl;
	}
}

void Engine::setInboundEventHandler(special_event_handler handler) {
	this->specialEventHandler = handler;
}
