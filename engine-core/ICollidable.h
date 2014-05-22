#pragma once
#include "engine-core.h"

#include <memory>

#include "common/Vector4.h"

enum class BoundsType {
	None = 0,
	Sphere = 1
};

class Bounds {
public:
	const BoundsType type = BoundsType::None;

	Bounds() : type(BoundsType::None) {}
	Bounds(BoundsType t) : type(t) {}

};

class BoundingSphere : public Bounds {
public:
	Common::Vector4 position;
	Common::Vector4 velocity;
	float radius;
	float mass;

	BoundingSphere() : Bounds(BoundsType::Sphere) {}
};

class COREDLL ICollidable {
public:
	virtual Common::Vector4 getGroupingParameter() const = 0;
	virtual bool collidesWith(const ICollidable*) const = 0;
	virtual void handleCollision(std::shared_ptr<const Bounds>, float dt) = 0;
	virtual std::shared_ptr<const Bounds> getBounds() const = 0;
	virtual unsigned int getPriority() const = 0;
};