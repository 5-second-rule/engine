#include "EngineInstance.h"


EngineInstance::EngineInstance(World *world, ObjectCtorTable *objectCtors) {
	this->world = world;
	this->objectCtors = objectCtors;
}

EngineInstance::~EngineInstance() {
}
