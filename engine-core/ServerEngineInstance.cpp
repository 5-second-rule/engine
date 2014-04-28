#include "ServerEngineInstance.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

static const char *testData = "This is the latest edition of the world from your local neighborhood server\n";

ServerEngineInstance::ServerEngineInstance(
	World *world,
	ObjectCtorTable *objectCtors,
	int minimumFrameTime)
		: EngineInstance(world, objectCtors, CommsProcessorRole::SERVER) {

	this->running = false;
}

ServerEngineInstance::~ServerEngineInstance() {
}

bool ServerEngineInstance::shouldContinueFrames() {
	return this->running;
}

void ServerEngineInstance::frame(int dt) {
	// get start time
	steady_clock::time_point start = steady_clock::now();

	this->processNetworkUpdates();
	this->world->update(dt);

	// TESTING CODE
	static int elapsedCount = 0;
	if (elapsedCount == 30)
	{
		elapsedCount = 0;
		comms->sendAnnouce();
	}

	comms->sendUpdates(testData, strlen(testData));

	// get end time
	steady_clock::time_point end = steady_clock::now();

	// sleep for remainder of dt
	sleep_for(microseconds(this->minimumFrameTime) - duration_cast<microseconds>(end - start));
}

void ServerEngineInstance::run(){
	// TODO no already running checks, not important right now
	this->running = true;
	EngineInstance::run();
}

void ServerEngineInstance::stop() {
	this->running = false;
}





