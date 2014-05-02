#pragma once

#include "engine-renderer.h"
#include "RenderableWorld.h"
#include "ModelData.h"
#include "engine-core/Engine.h"
#include "engine-core/Event.h"
#include "engine-core/ObjectCtorTable.h"

#include "renderer/Window.h"
#include "renderer/Input.h"
#include "renderer/Renderer.h"
#include "renderer/Texture.h"

#include <vector>

class RENDERDLL RenderingEngine : public Engine
{
private:
	Transmission::Input *input;
	Transmission::Window *window;
	Transmission::Renderer *renderer;
	RenderableWorld *renderableWorld;
	std::vector<ModelData> models;
	std::vector<Transmission::Texture *> textures;

protected:
	typedef Event* (*InputTranslatorFn)(Transmission::Input::Key, Transmission::Input::KeyState); // input handler, typedef for future changes
	InputTranslatorFn inputTranslator;

	void translateInput();

	virtual bool shouldContinueFrames();
	virtual void frame(float dt);

	virtual void tick(float dt);

public:
	RenderingEngine(
		RenderableWorld *world,
		ObjectCtorTable *objectCtors,
		void *appHandle);
	~RenderingEngine();

	int loadModel(char *filename);
	int loadTexture(char *filename);
	Transmission::Model * createModelFromIndex(size_t modelIndex, size_t textureIndex);
	void setInputTranslator(InputTranslatorFn translator);
};

