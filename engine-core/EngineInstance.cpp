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
	long lastFrameTime = 0;
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

	std::cout << "finsihed updates" << std::endl;
}

void EngineInstance::dispatchUpdate(QueueItem &item) {
	BufferBuilder *readBuffer = BufferBuilder::forReading(item.data, item.len);
	struct EventHeader *header = reinterpret_cast<struct EventHeader *>(readBuffer->getPointer());
	if (header->type == EventType::OBJECT_UPDATE) {
		readBuffer->reserve(sizeof(struct EventHeader));
		this->dispatchObjectUpdate(readBuffer);
	}
	else {
		// TODO handle directed events
	}
}

void EngineInstance::dispatchObjectUpdate(BufferBuilder *buffer) {
	buffer->reserve(sizeof(struct ObjectUpdateHeader));
	struct ObjectUpdateHeader *header = reinterpret_cast<struct ObjectUpdateHeader *>(buffer->getPointer());

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

	serializable->rehydrate(buffer);

	if (isNew) {
		world->insert(object);
	}
}
