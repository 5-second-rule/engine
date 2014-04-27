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
	static char tmpBuf[65536];
	memcpy(tmpBuf, item.data, item.len);
	tmpBuf[item.len] = '\0';
	std::cout << tmpBuf;
}

