#include "EngineInstance.h"


EngineInstance::EngineInstance(World *world, ObjectCtorTable *objectCtors) {
	this->world = world;
	this->objectCtors = objectCtors;
}

EngineInstance::~EngineInstance() {
}

void EngineInstance::run() {
	while (this->shouldContinueFrames()) {
		this->frame();
	}
}

bool EngineInstance::shouldContinueFrames() {
	return true;
}

void EngineInstance::frame() {
	// TODO do stuff
}
