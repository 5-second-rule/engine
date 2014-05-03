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
		if(_DEBUG) std::cout << "========= handling update =========" << std::endl;
		QueueItem update = this->networkUpdates.pop();
		dispatchUpdate(update);
		delete[] update.data;
	}

	if(_DEBUG) std::cout << "finished updates" << std::endl;
}

void Engine::dispatchUpdate(QueueItem &item) {
	BufferReader readBuffer(item.data, item.len);
	const struct EventHeader *header = reinterpret_cast<const struct EventHeader *>(readBuffer.getPointer());
	
<<<<<<< HEAD:engine-core/EngineInstance.cpp
	if ( EventType( header->type ) == EventType::OBJECT_UPDATE ) {
		readBuffer->finished(sizeof(struct EventHeader));
		this->dispatchObjectUpdate(readBuffer);
	} else if( EventType( header->type ) == EventType::ACTION ) {
		readBuffer->finished( sizeof( struct EventHeader ) );
		this->dispatchAction( readBuffer );
	} else if( EventType( header->type ) == EventType::SPECIAL ) {
		readBuffer->finished(sizeof(struct EventHeader));
=======
	if (header->type == EventType::OBJECT_UPDATE) {
		readBuffer.finished(sizeof(struct EventHeader));
		this->updateObject(readBuffer);

	} else if (header->type == EventType::SPECIAL) {
		readBuffer.finished(sizeof(struct EventHeader));
>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a:engine-core/Engine.cpp
		special_event_handler handler = this->specialEventHandler;
		if (handler != nullptr) {
			handler(readBuffer);
		}
	} else {
		DirectedEvent *directed = DirectedEvent::forReading();
		directed->deserialize(readBuffer);
		world->dispatchEvent(directed);

		// no delete for event, event queue on objects responsible
	}

}

void Engine::updateObject(BufferReader& buffer) {
	const struct ObjectUpdateHeader *header = reinterpret_cast<const struct ObjectUpdateHeader *>(buffer.getPointer());

	IHasHandle *object = this->world->get(&header->handle);
	bool isNew = false;
	if (object == nullptr) {
		object = this->objectCtors->invoke(header->objectType);
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

<<<<<<< HEAD:engine-core/EngineInstance.cpp
void EngineInstance::dispatchAction( BufferReader *buffer ) {
	const struct ActionHeader *header = reinterpret_cast<const struct ActionHeader *>(buffer->getPointer());
	
	//ActionEvent *evt = new ActionEvent( header->actionType, header->playerGuid );
	// test version, use above call for real thing
	ActionEvent *evt = new ActionEvent( header->actionType, gId, gIndex );
	
	this->delegate->HandleAction( evt );
}

void EngineInstance::sendOutboundEvent(Event *evt) {
=======
void Engine::sendOutboundEvent(Event *evt) {
>>>>>>> b996896406f70ec0227fa64c769607aed5e8056a:engine-core/Engine.cpp
	BufferBuilder *buffer = new BufferBuilder();
	evt->serialize(buffer);

	this->comms->sendUpdates(buffer->getBasePointer(), buffer->getSize());

	delete buffer;
}

void Engine::setInboundEventHandler(special_event_handler handler) {
	this->specialEventHandler = handler;
}
