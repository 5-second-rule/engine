#pragma once

#include "engine-renderer.h"
#include "engine-core/World.h"
#include "IRenderable.h"

#include <vector>

class RENDERDLL RenderableWorld : public World
{
private:
	GCHandleVector<IRenderable>* renderable;

public:
	RenderableWorld();
	~RenderableWorld();

	virtual void insert(IHasHandle *object);
	virtual void garbageCollectWorld();

	void renderAll();
};

