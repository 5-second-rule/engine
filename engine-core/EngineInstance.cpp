#include "EngineInstance.h"
#include <chrono>

#include <iostream>

using namespace std::chrono;

EngineInstance::EngineInstance(
		World *world, 
		ObjectCtorTable *objectCtors, 
		CommsProcessorRole role) {
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
	long long lastFrameTime = 0;
	while (this->shouldContinueFrames()) {
		steady_clock::time_point start = steady_clock::now();

		this->frame((int)lastFrameTime);
		
		steady_clock::time_point end = steady_clock::now();
		lastFrameTime = duration_cast<microseconds>(end - start).count();
	}
}

bool EngineInstance::shouldContinueFrames() {
	return true;
}

void EngineInstance::frame(int dt) {
	// TODO do stuff
}

void EngineInstance::processNetworkUpdates() {
	// bring new updates forward
	this->networkUpdates.swap();

	while (!this->networkUpdates.readEmpty()) {
		std::cout << "========= handling update =========" << std::endl;
		QueueItem update = this->networkUpdates.pop();
		dispatchUpdate(update);
		delete[] update.data;
	}

	std::cout << "finished updates" << std::endl;
}

void EngineInstance::dispatchUpdate(QueueItem &item) {
	BufferReader *readBuffer = new BufferReader(item.data, item.len);
	const struct EventHeader *header = reinterpret_cast<const struct EventHeader *>(readBuffer->getPointer());
	
	if (header->type == EventType::OBJECT_UPDATE) {
		readBuffer->finished(sizeof(struct EventHeader));
		this->dispatchObjectUpdate(readBuffer);

	} else if (header->type == EventType::SPECIAL) {
		readBuffer->finished(sizeof(struct EventHeader));
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

	delete readBuffer;
}

void EngineInstance::dispatchObjectUpdate(BufferReader *buffer) {
	const struct ObjectUpdateHeader *header = reinterpret_cast<const struct ObjectUpdateHeader *>(buffer->getPointer());

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

	buffer->finished(sizeof(struct ObjectUpdateHeader));
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
