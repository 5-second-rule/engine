#pragma once

#include "engine-renderer.h"
#include "RenderableWorld.h"
#include "ModelData.h"
#include "engine-core/EngineInstance.h"
#include "engine-core/ObjectCtorTable.h"

#include "renderer/Window.h"
#include "renderer/Renderer.h"

#include <vector>

class RENDERDLL RenderingEngineInstance : public EngineInstance
{
private:
	Window *window;
	Renderer *renderer;
	RenderableWorld *renderableWorld;
	std::vector<ModelData> modelData;

protected:
	virtual bool shouldContinueFrames();
	virtual void frame();

public:
	RenderingEngineInstance(
		RenderableWorld *world,
		ObjectCtorTable *objectCtors,
		void *appHandle);
	~RenderingEngineInstance();

	int loadModelFile(char *filename);
	Model * createModelFromIndex(int modelIndex);
};

