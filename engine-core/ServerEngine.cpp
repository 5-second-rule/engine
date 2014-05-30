#include "ServerEngine.h"
#include <chrono>
#include <thread>
#include <sstream>

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
{
	this->commandLine.registerCommand( "printWorld", new PrintWorld(this->world) );
	Command* exit = new Exit( this );
	this->commandLine.registerCommand( "exit", exit );
	this->commandLine.registerCommand( "quit", exit );
	this->commandLine.registerCommand( "stop", exit );
	this->commandLine.registerCommand( "setDebugLevel", new Debug(this) );
}

void ServerEngine::tick(float dt) {
	static int annouceCount = 0;

	this->processNetworkUpdates();
	Engine::tick(dt);
	this->world->broadcastUpdates(comms);

	if( annouceCount == 25 ) {
		comms->sendAnnouce();
		annouceCount = 0;
	} else ++annouceCount;

	this->commandLine.update();

}


void ServerEngine::frame(float dt) {
	// sleep for remainder of dt
	sleep_for(duration_cast<nanoseconds>(float_seconds(this->secondsPerTick - dt)));
}

// command-line extensions for ServerEngine
void PrintWorld::execute( std::string args ) {
	for( size_t i = 0; i < this->world->getObjects()->size(); ++i ) {
		std::cout << std::endl << this->world->getObjects()->at( i )->toString() << std::endl;
	}
}

void Exit::execute( std::string args ) {
	this->engine->stop();
	std::cout << "Press Enter to close server";
}

void Debug::execute( std::string args ) {
	if( args == "" ) {
		std::cout << "Setting Debug level requires one argument" << std::endl;
		return;
	}

	std::stringstream ss( args );
	ss >> this->engine->debugLevel;
}