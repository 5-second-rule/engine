#include "ServerEngineInstance.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

static const char *testData = "This is the latest edition of the world from your local neighborhood server\n";

ServerEngineInstance::ServerEngineInstance(World *world, ObjectCtorTable *objectCtors, float secondsPerTick)
	: EngineInstance(world, objectCtors, CommsProcessorRole::SERVER)
	, secondsPerTick(secondsPerTick)
{}

ServerEngineInstance::~ServerEngineInstance() {}


void ServerEngineInstance::frame(float dt) {
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

	this->world->broadcastUpdates(comms);

	// get end time
	steady_clock::time_point end = steady_clock::now();

	// sleep for remainder of dt
	sleep_for(microseconds(this->minimumFrameTime) - duration_cast<microseconds>(end - start));
}

void ServerEngineInstance::run(){

	// TEST HACK
	IHasHandle * obj = this->objectCtors->invoke(0);
	world->allocateHandle(obj, HandleType::GLOBAL);
	world->insert(obj);

	EngineInstance::run();
}





