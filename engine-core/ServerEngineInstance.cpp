#include "ServerEngineInstance.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

ServerEngineInstance::ServerEngineInstance(World *world, ObjectCtorTable *objectCtors, float secondsPerTick)
	: EngineInstance(world, objectCtors, CommsProcessorRole::SERVER)
	, secondsPerTick(secondsPerTick)
{}

ServerEngineInstance::~ServerEngineInstance() {}

void ServerEngineInstance::tick(float dt) {
	this->processNetworkUpdates();
	EngineInstance::tick(dt);
	this->world->broadcastUpdates(comms);
}


void ServerEngineInstance::frame(float dt) {
	// sleep for remainder of dt
	sleep_for(duration<float>(this->secondsPerTick - dt));
}

void ServerEngineInstance::run(){

	// TEST HACK
	IHasHandle * obj = this->objectCtors->invoke(0);
	world->allocateHandle(obj, HandleType::GLOBAL);
	world->insert(obj);

	EngineInstance::run();
}





