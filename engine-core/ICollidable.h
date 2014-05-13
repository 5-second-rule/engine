#pragma once

#include "engine-core.h"

#include "common/Vector4.h"

struct BoundingSphere {
	Common::Vector4 position;
	float radius;
};

class COREDLL ICollidable {
public:
	virtual Common::Vector4* getGroupingParameter() = 0;
	virtual bool collidesWith(ICollidable* target) = 0;
	virtual void handleCollision(ICollidable* target) = 0;
	virtual BoundingSphere getBounds() = 0;
	virtual unsigned int getPriority() = 0;
};