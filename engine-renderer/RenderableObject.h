#pragma once

#include "engine-renderer.h"
#include "renderer/Model.h"
#include "engine-renderer/IRenderable.h"
#include "engine-core/BaseObject.h"

class RENDERDLL RenderableObject : public IRenderable, public BaseObject
{
private:
	Transmission::Model *model;

public:
	RenderableObject(int objectType, Transmission::Model *model);
	~RenderableObject();

	virtual void render();
};

