#pragma once

#include "engine-renderer.h"
#include "RenderableWorld.h"
#include "ModelData.h"
#include "PlayerCameraHandler.h"
#include "engine-core/Engine.h"
#include "engine-core/Event.h"
#include "engine-core/ConstructorTable.h"

#include "renderer/Window.h"
#include "renderer/Input.h"
#include "renderer/Renderer.h"
#include "renderer/Texture.h"
#include "renderer/Shader.h"
#include "renderer/Sounder.h"

#include "InputAdapter.h"
#include "SoundObject.h"

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
	Transmission::Sounder *sounder;
	RenderableWorld *renderableWorld;
	std::vector<ModelData> models;
	std::vector<Transmission::Texture *> textures;
	std::vector<Transmission::Texture *> bumpMaps;
	std::vector<Shader *> pixelShaders;
	std::vector<Shader *> vertexShaders;
	std::vector<Sound *> sounds;
	InputAdapter inputAdapter;
	PlayerCameraHandler *cameraHandler;
	
protected:
	void translateInput();

	virtual bool shouldContinueFrames();
	virtual void frame(float dt);

	virtual void tick(float dt);

public:
	RenderingEngine( RenderableWorld *world, 
									 ConstructorTable<BaseObject> *objectCtors, 
									 ConstructorTable<ActionEvent>* actionCtors,
									 ConstructorTable<SoundObject>* soundCtors,
									 void *appHandle,
									 PlayerCameraHandler *cameraHandler,
									 char* defaultVertex,
									 char* defaultPixel,
									 const wchar_t* windowName);
	~RenderingEngine();

	ConstructorTable<SoundObject>* soundCtors;

	int loadModel(char *filename);
	int loadModel(char *filename, bool centered);
	int loadTexture(char *filename);
	int loadBumpMap(char *filename);
	int loadPixelShader( char *filename );
	int loadVertexShader( char *filename );
	int loadSound( char *filename );
	Model * createModelFromIndex(size_t modelIndex, size_t textureIndex);
	Model * createModelFromIndex(size_t modelIndex, size_t textureIndex, size_t bumpIndex);
	Model * createModelFromIndex(size_t modelIndex, size_t textureIndex, size_t vertexShader, size_t pixelShader);
	Model * createModelFromIndex(size_t modelIndex, size_t textureIndex, size_t bumpIndex, size_t vertexShader, size_t pixelShader);

	Model * create2DModelFromVertices(Vertex *v, int numVertices, Index *i, int numIndices, Texture *texture);
	Model * create2DModelFromVertices(Vertex *v, int numVertices, Index *i, int numIndices, Texture *texture, Shader *vs, Shader *ps);
	Model * create2DModelFromScratch(Vertex *v, int numVertices, Index *i, int numIndices, char *textureFile, std::vector<Transmission::Texture *> textureStorage, bool transparent);
	Model * create2DModelFromScratch(Vertex *v, int numVertices, Index *i, int numIndices, char *textureFile, std::vector<Transmission::Texture *> textureStorage, Shader *vs, Shader *ps, bool transparent);

	Sound * createSoundFromIndex( size_t soundIndex );
  
	virtual void dispatchSound( SoundEvent *evt );

	void waitForServer();

	IRenderingEngineDelegate* renderingDelegate;
};

