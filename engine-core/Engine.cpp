#include "Engine.h"

#include <iostream>

#define TICKS_PER_SEC 25

using namespace std::chrono;

Engine::Engine(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role)
	: secondsPerTick(1.0f / (float)TICKS_PER_SEC)
{
	this->world = world;
	this->objectCtors = objectCtors;

	// Set up network
	this->comms = new CommsProcessor(role);
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
		//if(_DEBUG) std::cout << "========= handling update =========" << std::endl;
		QueueItem update = this->networkUpdates.pop();
		dispatchUpdate(update);
		delete[] update.data;
	}

	//if(_DEBUG) std::cout << "finished updates" << std::endl;
}

void Engine::dispatchUpdate(QueueItem &item) {
	BufferReader readBuffer(item.data, item.len);
	const struct EventHeader *header = reinterpret_cast<const struct EventHeader *>(readBuffer.getPointer());
	readBuffer.finished(sizeof(struct EventHeader));

	switch (EventType(header->type)) {
	case EventType::REGISTER_PLAYER:
		if (_DEBUG) std::cout << "player registration inbound" << std::endl;
		this->handleRegistrationRequest(readBuffer);
		break;
	case EventType::REGISTER_PLAYER_RESPONSE:
		this->handleRegistrationResponse(readBuffer);
		break;
	}

	if (!waitingForRegistration) {
		switch (EventType(header->type)) {
		case EventType::OBJECT_UPDATE:
			this->updateObject(readBuffer);
			break;
		case EventType::ACTION:
			if (_DEBUG) std::cout << "action!" << std::endl;
			this->dispatchAction(&readBuffer);
			break;
		case EventType::SPECIAL:
			if (this->specialEventHandler != nullptr) {
				this->specialEventHandler(readBuffer);
			}
			break;
		default:
			//TODO log bad event
			break;
		}
	}
}

void Engine::handleRegistrationRequest(BufferReader& buffer) {
	const struct RegistrationRequestHeader *header = reinterpret_cast<const struct RegistrationRequestHeader *>(buffer.getPointer());

	auto place = this->playerMap.find(header->playerGuid);
	Response response = Response::FAIL;
	if (place == this->playerMap.end()) {
		response = Response::OK;
		// spot is available, yay!

		// HACK make first model for now
		IHasHandle * obj = this->objectCtors->invoke(0);
		world->allocateHandle(obj, HandleType::GLOBAL);
		world->insert(obj);

		this->playerMap[header->playerGuid] = obj->getHandle();

		if (_DEBUG) std::cout << "=> player registered" << std::endl;
	} else {
		if (_DEBUG) std::cout << "=> player NOT registered #fail" << std::endl;
	}

	BufferBuilder responseBuffer = BufferBuilder();
	responseBuffer.reserve(sizeof(struct EventHeader));
	responseBuffer.reserve(sizeof(struct RegistrationResponseHeader));

	responseBuffer.allocate();

	((struct EventHeader*)responseBuffer.getPointer())->type = static_cast<int>(EventType::REGISTER_PLAYER_RESPONSE);
	responseBuffer.filled();

	struct RegistrationResponseHeader *responseHeader = (struct RegistrationResponseHeader*)responseBuffer.getPointer();
	responseHeader->response = (int)response;
	responseHeader->responseTag = header->responseTag;
	responseBuffer.filled();

	comms->sendUpdates(responseBuffer.getBasePointer(), responseBuffer.getSize());
}

// HACK not really safe but good enough for now
// what if server response lost? Object created on server but client doesn't know
void Engine::handleRegistrationResponse(BufferReader& buffer) {
	const struct RegistrationResponseHeader *header = reinterpret_cast<const struct RegistrationResponseHeader *>(buffer.getPointer());

	if (header->responseTag == this->waitingRegistration.responseTag
		&& header->response == (int)Response::OK) {
		// matches;
		this->waitingForRegistration = false;
		this->localPlayers.push_back(this->waitingRegistration.playerGuid);
	}
}

// HACK not really safe either, no prevention of double call
void Engine::registerPlayer(bool wait) {
	this->waitingForRegistration = wait;

	this->waitingRegistration.playerGuid = rand();
	this->waitingRegistration.responseTag = rand();

	BufferBuilder responseBuffer = BufferBuilder();
	responseBuffer.reserve(sizeof(struct EventHeader));
	responseBuffer.reserve(sizeof(struct RegistrationRequestHeader));

	responseBuffer.allocate();

	((struct EventHeader*)responseBuffer.getPointer())->type = static_cast<int>(EventType::REGISTER_PLAYER);
	responseBuffer.filled();

	struct RegistrationRequestHeader *responseHeader = (struct RegistrationRequestHeader*)responseBuffer.getPointer();
	*responseHeader = this->waitingRegistration;
	responseBuffer.filled();

	comms->sendUpdates(responseBuffer.getBasePointer(), responseBuffer.getSize());
}

unsigned int Engine::getLocalPlayerGuid(unsigned int playerIndex) {
	if (playerIndex < 0 || playerIndex >= this->localPlayers.size()) {
		// TODO do something
		return 0;
	}

	return this->localPlayers[playerIndex];
}

void Engine::updateObject(BufferReader& buffer) {
	const struct ObjectUpdateHeader *header = reinterpret_cast<const struct ObjectUpdateHeader *>(buffer.getPointer());

	IHasHandle *object = this->world->get(&header->handle);
	bool isNew = false;
	if (object == nullptr) {
		object = this->objectCtors->invoke(header->objectType);
		object->setHandle(header->handle);
		isNew = true;
	}

	ISerializable *serializable = dynamic_cast<ISerializable *>(object);
	if (serializable == nullptr) {
		throw new std::runtime_error("Expected serializable object, but it's not; wat.");
	}

	buffer.finished(sizeof(struct ObjectUpdateHeader));
	serializable->deserialize(buffer);

	if (isNew) {
		world->insert(object);
	}
}

void Engine::dispatchAction( BufferReader *buffer ) {
	const struct ActionHeader *header = reinterpret_cast<const struct ActionHeader *>(buffer->getPointer());
	buffer->finished( sizeof( struct ActionHeader ) );

	ActionEvent *evt = this->delegate->MakeActionEvent( header->actionType, header->playerGuid, buffer->getPointer() );

	auto playerHandle = this->playerMap.find(header->playerGuid);

	if (playerHandle != this->playerMap.end()) {
		this->world->dispatchEvent(evt, playerHandle->second);
	} else {
		std::cout << "WARN: unknown player attempted an action" << std::endl;
	}
}

void Engine::sendOutboundEvent(Event *evt) {
	BufferBuilder buffer;
	evt->serialize(buffer);

	this->comms->sendUpdates(buffer.getBasePointer(), buffer.getSize());
}

void Engine::setInboundEventHandler(special_event_handler handler) {
	this->specialEventHandler = handler;
}
