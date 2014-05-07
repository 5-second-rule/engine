#pragma once

#include "engine-renderer.h"
#include "RenderableWorld.h"
#include "ModelData.h"
#include "engine-core/Engine.h"
#include "engine-core/Event.h"
#include "engine-core/ConstructorTable.h"

#include "renderer/Window.h"
#include "renderer/Input.h"
#include "renderer/Renderer.h"
#include "renderer/Texture.h"

#include "InputAdapter.h"

#include <vector>

class RENDERDLL IRenderingEngineDelegate {
public:
	virtual std::vector<Event *> inputTranslator(InputAdapter *inputAdapter) = 0;
};

class RENDERDLL RenderingEngine : public Engine
{
private:
	Transmission::Input *input;
	Transmission::Window *window;
	Transmission::Renderer *renderer;
	RenderableWorld *renderableWorld;
	std::vector<ModelData> models;
	std::vector<Transmission::Texture *> textures;
	InputAdapter inputAdapter;
	
protected:
	void translateInput();

	virtual bool shouldContinueFrames();
	virtual void frame(float dt);

	virtual void tick(float dt);

public:
	RenderingEngine(RenderableWorld *world, ConstructorTable<IHasHandle> *objectCtors, void *appHandle);
	~RenderingEngine();
	int loadModel(char *filename);
	int loadTexture(char *filename);
	Transmission::Model * createModelFromIndex(size_t modelIndex, size_t textureIndex);
	void waitForServer();

	IRenderingEngineDelegate* renderingDelegate;
};

