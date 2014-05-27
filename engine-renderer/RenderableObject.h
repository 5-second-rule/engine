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

	void setPosition(const float pos[3]);
	void setScale(const float scale);
protected:
	void setRotation(const Vector4& rot);
};
