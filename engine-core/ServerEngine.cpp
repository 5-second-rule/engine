#include "ServerEngine.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

ServerEngine::ServerEngine(
	World *world,
	ConstructorTable<BaseObject> *objectCtors,
	ConstructorTable<ActionEvent>* actionCtors,
	float secondsPerTick
)
	: Engine(world, objectCtors, actionCtors, CommsProcessorRole::SERVER)
	, secondsPerTick(secondsPerTick)
{}

void ServerEngine::tick(float dt) {
	static int annouceCount = 0;

	this->processNetworkUpdates();
	Engine::tick(dt);
	this->world->broadcastUpdates(comms);

	if( annouceCount == 25 ) {
		comms->sendAnnouce();
		annouceCount = 0;
	}
	else
		++annouceCount;

}


void ServerEngine::frame(float dt) {
	// sleep for remainder of dt
	sleep_for(duration_cast<nanoseconds>(float_seconds(this->secondsPerTick - dt)));
}

void ServerEngine::run(){
	Engine::run();
}