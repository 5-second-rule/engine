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
	this->paused = false;
	this->commandLine.registerCommand( "printWorld", new PrintWorld(this->world) );
	Command* exit = new Exit( this );
	this->commandLine.registerCommand( "exit", exit );
	this->commandLine.registerCommand( "quit", exit );
	this->commandLine.registerCommand( "stop", exit );
	this->commandLine.registerCommand( "setDebugLevel", new Debug(this) );
	this->commandLine.registerCommand( "pause", new Pause( this ));
}

void ServerEngine::tick(float dt) {
	static int annouceCount = 0;

	if( !this->paused ) {
		this->processNetworkUpdates();
		Engine::tick( dt );
		this->world->broadcastUpdates( comms );
	}

	// garbagage collect objects marked as gc after sending this update out to clients
	this->world->garbageCollectWorld();

	if( annouceCount == 25 ) { // about every 2 seconds
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
	this->world->printWorld();
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

void Pause::execute( std::string args ) {
	if( args == "" ) {
		std::cout << "Pause requires an argument of on or off" << std::endl;
		return;
	}

	if( args == "on" || args == "true" || args == "yes" ) {
		this->engine->paused = true;
	} else if( args == "off" || args == "false" || args == "no" ) {
		this->engine->paused = false;
	} else {
		std::cout << args << " is an invalid setting (on/yes/true or off/no/false)" << std::endl;
	}
}