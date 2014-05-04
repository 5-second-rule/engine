#include "ServerEngine.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

ServerEngine::ServerEngine(World *world, ObjectCtorTable *objectCtors, float secondsPerTick)
	: Engine(world, objectCtors, CommsProcessorRole::SERVER)
	, secondsPerTick(secondsPerTick)
{}

ServerEngine::~ServerEngine() {}

void ServerEngine::tick(float dt) {
	this->processNetworkUpdates();
	Engine::tick(dt);
	this->world->broadcastUpdates(comms);
}


void ServerEngine::frame(float dt) {
	// sleep for remainder of dt
	sleep_for(duration_cast<nanoseconds>(float_seconds(this->secondsPerTick - dt)));
}

void ServerEngine::run(){
	Engine::run();
}