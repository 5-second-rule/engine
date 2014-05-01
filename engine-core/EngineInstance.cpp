#include "EngineInstance.h"

#include <iostream>

#define TICKS_PER_SEC 25

using namespace std::chrono;

EngineInstance::EngineInstance(
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

EngineInstance::~EngineInstance() {
	delete this->comms;
}

void EngineInstance::run() {

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

void EngineInstance::stop() {
	running = false;
}

bool EngineInstance::shouldContinueFrames() {
	return running;
}

bool EngineInstance::checkForTick(float dt) {
	return dt >= secondsPerTick;
}

void EngineInstance::tick(float dt) {
	this->world->update(dt);
}

void EngineInstance::processNetworkUpdates() {
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

void EngineInstance::dispatchUpdate(QueueItem &item) {
	BufferReader readBuffer(item.data, item.len);
	const struct EventHeader *header = reinterpret_cast<const struct EventHeader *>(readBuffer.getPointer());
	
	if (header->type == EventType::OBJECT_UPDATE) {
		readBuffer.finished(sizeof(struct EventHeader));
		this->updateObject(readBuffer);

	} else if (header->type == EventType::SPECIAL) {
		readBuffer.finished(sizeof(struct EventHeader));
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

void EngineInstance::updateObject(BufferReader& buffer) {
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

void EngineInstance::sendOutboundEvent(Event *evt) {
	BufferBuilder *buffer = new BufferBuilder();
	evt->serialize(buffer);

	this->comms->sendUpdates(buffer->getBasePointer(), buffer->getSize());

	delete buffer;
}

void EngineInstance::setInboundEventHandler(special_event_handler handler) {
	this->specialEventHandler = handler;
}
