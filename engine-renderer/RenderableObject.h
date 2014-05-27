#pragma once

#include "engine-renderer.h"
#include "renderer/Model.h"
#include "engine-renderer/IRenderable.h"
#include "engine-core/BaseObject.h"

class RENDERDLL RenderableObject : public IRenderable
{
private:
	Transmission::Model *model;

public:
	RenderableObject(Transmission::Model *model);
	virtual ~RenderableObject();

	virtual void render();

protected:
	IMoveable* getMoveable();
};
