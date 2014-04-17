#pragma once

#include "engine-renderer.h"
#include "engine-core/World.h"
#include "IRenderable.h"

#include <vector>

class RENDERDLL RenderableWorld : public World
{
private:
	std::vector<Handle> renderable;

public:
	RenderableWorld();
	~RenderableWorld();

	virtual void insert(IHasHandle *object);
};

