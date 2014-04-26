#pragma once

#include "engine-renderer.h"
#include "RenderableWorld.h"
#include "ModelData.h"
#include "engine-core/EngineInstance.h"
#include "engine-core/ObjectCtorTable.h"

#include "renderer/Window.h"
#include "renderer/Renderer.h"
#include "renderer/Texture.h"

#include <vector>

class RENDERDLL RenderingEngineInstance : public EngineInstance
{
private:
	Transmission::Window *window;
	Transmission::Renderer *renderer;
	RenderableWorld *renderableWorld;
	std::vector<ModelData> models;
	std::vector<Transmission::Texture *> textures;

protected:
	virtual bool shouldContinueFrames();
	virtual void frame(int dt);

public:
	RenderingEngineInstance(
		RenderableWorld *world,
		ObjectCtorTable *objectCtors,
		void *appHandle);
	~RenderingEngineInstance();

	int loadModel(char *filename);
	int loadTexture(char *filename);
	Transmission::Model * createModelFromIndex(int modelIndex, int textureIndex);
};

