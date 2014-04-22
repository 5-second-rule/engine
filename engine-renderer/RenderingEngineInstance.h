#pragma once

#include "engine-renderer.h"
#include "engine-core/EngineInstance.h"
#include "engine-renderer/RenderableWorld.h"
#include "engine-core/ObjectCtorTable.h"

#include "renderer/Window.h"
#include "renderer/Renderer.h"

class RENDERDLL RenderingEngineInstance : public EngineInstance
{
private:
	Window *window;
	Renderer *renderer;
	RenderableWorld *renderableWorld;

protected:
	virtual bool shouldContinueFrames();
	virtual void frame();

public:
	RenderingEngineInstance(
		RenderableWorld *world,
		ObjectCtorTable *objectCtors,
		void *appHandle);
	~RenderingEngineInstance();
};

