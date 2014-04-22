#pragma once

#include "engine-renderer.h"
#include "renderer/Model.h"
#include "engine-renderer/IRenderable.h"
#include "engine-core/IHasHandle.h"

class RENDERDLL RenderableObject : public IRenderable, public IHasHandle
{
private:
	Model *model;
	Handle handle;

public:
	RenderableObject(Model *model);
	~RenderableObject();

	virtual Handle getHandle();
	virtual void setHandle(Handle handle);
	virtual void render();
};

