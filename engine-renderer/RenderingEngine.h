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
#include "renderer/Shader.h"

#include "InputAdapter.h"

#include <vector>

using namespace Transmission;

class RENDERDLL IRenderingEngineDelegate {
public:
	virtual std::vector<Event *> inputTranslator(InputAdapter *inputAdapter) = 0;
};

class RENDERDLL RenderingEngine : public Engine
{
private:
	const Transmission::Input *input;
	Transmission::Window *window;
	Transmission::Renderer *renderer;
	RenderableWorld *renderableWorld;
	std::vector<ModelData> models;
	std::vector<Transmission::Texture *> textures;
	std::vector<Shader *> pixelShaders;
	std::vector<Shader *> vertexShaders;
	InputAdapter inputAdapter;
	
protected:
	void translateInput();

	virtual bool shouldContinueFrames();
	virtual void frame(float dt);

	virtual void tick(float dt);

public:
	RenderingEngine(RenderableWorld *world, ConstructorTable<BaseObject> *objectCtors, ConstructorTable<ActionEvent>* a, void *appHandle);
	~RenderingEngine();
	int loadModel(char *filename);
	int loadTexture(char *filename);
	int loadPixelShader( char *filename );
	int loadVertexShader( char *filename );
	Transmission::Model * createModelFromIndex(size_t modelIndex, size_t textureIndex);
	Model * createModelFromIndex( size_t modelIndex, size_t textureIndex, size_t vertexShader, size_t pixelShader );
	void waitForServer();

	IRenderingEngineDelegate* renderingDelegate;
};

