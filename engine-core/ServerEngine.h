#pragma once
#include "engine-core.h"

#include "Engine.h"

#include "CommandLine.h"

class COREDLL ServerEngine : public Engine
{
	friend class Debug;
	friend class Pause;
private:
	const float secondsPerTick;
	bool paused;

protected:

	virtual void tick(float dt);
	virtual void frame(float dt);

public:
	ServerEngine(
		World *world,
		ConstructorTable<BaseObject> *objectCtors,
		ConstructorTable<ActionEvent>*,
		float minimumFrameTime);
	CommandLine commandLine;
};

// command-line extensions for ServerEngine
class PrintWorld : public Command {
private:
	World* world;
public:
	PrintWorld( World* world ) : world( world ) {}
	void execute( std::string args );
};

class Exit : public Command {
private:
	Engine* engine;
public:
	Exit( Engine* engine ) : engine( engine ) {}
	void execute( std::string args );
};

class Debug : public Command {
private:
	ServerEngine* engine;
public:
	Debug( ServerEngine* engine ) : engine( engine ) {}
	void execute( std::string args );
};

class Pause : public Command {
private:
	ServerEngine* engine;
public:
	Pause( ServerEngine* engine ) : engine( engine ) {}
	void execute( std::string args );
};