#pragma once
#include "engine-core.h"

#include <memory>

#include "common/Vector4.h"

enum class BoundsType {
	None = 0,
	Sphere = 1
};

struct Bounds {
	const BoundsType type = BoundsType::None;
};

struct BoundingSphere : Bounds {
	const BoundsType type = BoundsType::Sphere;
	Common::Vector4 position;
	Common::Vector4 velocity;
	float radius;
	float mass;
};

class COREDLL ICollidable {
public:
	virtual Common::Vector4 getGroupingParameter() const = 0;
	virtual bool collidesWith(const ICollidable*) const = 0;
	virtual void handleCollision(std::shared_ptr<const Bounds>, float dt) = 0;
	virtual std::shared_ptr<const Bounds> getBounds() const = 0;
	virtual unsigned int getPriority() const = 0;
};