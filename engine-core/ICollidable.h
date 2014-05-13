#pragma once

#include "engine-core.h"

#include "common/Vector4.h"

class COREDLL ICollidable {
public:
	Common::Vector4* getGroupingParameter();
	bool collidesWith(ICollidable* target);
	void handleCollision(ICollidable* target);
	void* getBounds();
	unsigned int getPriority();
};