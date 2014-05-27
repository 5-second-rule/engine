#include "RenderingEngine.h"

// constants
static const char *testData = "This is a client update!!!\n";

RenderingEngine::RenderingEngine( RenderableWorld *world,
																	ConstructorTable<BaseObject> *objectCtors,
																	ConstructorTable<ActionEvent> *actionCtors,
																	ConstructorTable<SoundObject> *soundCtors,
																	void *appHandle,
																	PlayerCameraHandler *cameraHandler,
																	char* defaultVertex,
																	char* defaultPixel) 
		: Engine(world, objectCtors, actionCtors, CommsProcessorRole::CLIENT) {
	this->window = Window::createWindow(appHandle);
	this->input = this->window->getInput();
	this->renderer = Renderer::createRenderer(this->window, defaultVertex, defaultPixel);
	this->renderer->getTimer()->StartTimer();
	this->sounder = Sounder::createSounder( this->window );
	this->renderableWorld = world;
	this->inputAdapter = InputAdapter();
	this->inputAdapter.setInput(this->input);
	this->cameraHandler = cameraHandler;
	this->soundCtors = soundCtors;
}

RenderingEngine::~RenderingEngine() {}

void RenderingEngine::translateInput() {
	std::vector<Event *> inputEventVector = this->renderingDelegate->inputTranslator(&this->inputAdapter);
	std::vector<Event *>::iterator it;
	for (it = inputEventVector.begin(); it != inputEventVector.end(); ++it) {
		this->comms->sendEvent(*it);
	}
}

bool RenderingEngine::shouldContinueFrames() {
	Window::MessageType t;
	while ((t = this->window->getMessage()) != Window::MessageType::None) {
		if (t == Window::MessageType::Quit) return false;
	}

	return true;
}

void RenderingEngine::tick(float dt) {
	this->processNetworkUpdates();
	//Engine::tick(dt);
	this->translateInput();
}

void RenderingEngine::frame(float dt) {
	// HACK to only player 0
	if (this->localPlayers.size() > 0) {
		IHasHandle *playerObject = this->world->get(this->playerMap[this->localPlayers[0]]);
		if (playerObject != nullptr) {
			this->cameraHandler->updateFor(playerObject);
			Camera *camera = renderer->getCamera();
			camera->set(this->cameraHandler->position, this->cameraHandler->lookAt);
		}
	}

	renderer->clearFrame();
	this->renderableWorld->renderAll();
	renderer->drawFrame();
}

int RenderingEngine::loadModel(char *filename) {
	return this->loadModel(filename, true);
}

int RenderingEngine::loadModel(char *filename, bool centered) {
	ModelData data;

	if (this->renderer->loadModelFile(
			filename, 
			&data.vertexBuffer, 
			&data.indexBuffer,
			centered
			)
		) {
		this->models.push_back(data);
		return this->models.size() - 1;
	}
	
	return -1;
}

int RenderingEngine::loadTexture(char *filename) {
	Texture *texture = this->renderer->createTextureFromFile(filename);

	// TODO validate this error check
	if (texture != nullptr) {
		this->textures.push_back(texture);
		return this->textures.size() - 1;
	}

	return -1;
}

int RenderingEngine::loadBumpMap(char *filename) {
	Texture *bumpTexture = this->renderer->createTextureFromFile(filename);

	// TODO validate this error check
	if (bumpTexture != nullptr) {
		this->bumpMaps.push_back(bumpTexture);
		return this->bumpMaps.size() - 1;
	}

	return -1;
}

int RenderingEngine::loadPixelShader( char *filename ) {
	Shader *shader = this->renderer->createPixelShader( filename );

	// TODO validate this error check
	if( shader != nullptr ) {
		this->pixelShaders.push_back( shader );
		return this->pixelShaders.size() - 1;
	}

	return -1;
}

int RenderingEngine::loadVertexShader( char *filename ) {
	Shader *shader = this->renderer->createVertexShader( filename );

	// TODO validate this error check
	if( shader != nullptr ) {
		this->vertexShaders.push_back( shader );
		return this->vertexShaders.size() - 1;
	}

	return -1;
}

int RenderingEngine::loadSound( char *filename ) {
	Sound *sound = this->sounder->createSoundFromFile( filename );

	// TODO validate this error check
	if( sound != nullptr ) {
		this->sounds.push_back( sound );
		return this->sounds.size() - 1;
	}

	return -1;
}

Model * RenderingEngine::createModelFromIndex(size_t modelIndex, size_t textureIndex) {
	if (modelIndex < 0 || modelIndex >= this->models.size()) {
		throw std::runtime_error("Model index out of range.");
	} else if (textureIndex < 0 || textureIndex >= this->textures.size()) {
		throw std::runtime_error("Texture index out of range.");
	}

	ModelData data = this->models.at(modelIndex);
	Texture *texture = this->textures.at(textureIndex);

	return this->renderer->createModel(data.vertexBuffer, data.indexBuffer, texture);
}

Model * RenderingEngine::createModelFromIndex(size_t modelIndex, size_t textureIndex, size_t bumpIndex) {
	if (modelIndex < 0 || modelIndex >= this->models.size()) {
		throw std::runtime_error("Model index out of range.");
	}
	else if (textureIndex < 0 || textureIndex >= this->textures.size()) {
		throw std::runtime_error("Texture index out of range.");
	}
	else if (bumpIndex < 0 || bumpIndex >= this->bumpMaps.size()) {
		throw std::runtime_error("BumpMap index out of range.");
	}

	ModelData data = this->models.at(modelIndex);
	Texture *texture = this->textures.at(textureIndex);
	Texture *bumpMap = this->bumpMaps.at(bumpIndex);

	return this->renderer->createModel(data.vertexBuffer, data.indexBuffer, texture, bumpMap);
}

Model * RenderingEngine::createModelFromIndex( size_t modelIndex, size_t textureIndex, size_t vertexShader, size_t pixelShader ) {
	if( modelIndex < 0 || modelIndex >= this->models.size() ) {
		throw std::runtime_error( "Model index out of range." );
	} else if( textureIndex < 0 || textureIndex >= this->textures.size() ) {
		throw std::runtime_error( "Texture index out of range." );
	}

	ModelData data = this->models.at( modelIndex );
	Texture *texture = this->textures.at( textureIndex );
	Shader* vertex = this->vertexShaders.at( vertexShader );
	Shader* pixel = this->pixelShaders.at( pixelShader );

	return this->renderer->createModel( data.vertexBuffer, data.indexBuffer, texture, vertex, pixel );
}

Model * RenderingEngine::createModelFromIndex(size_t modelIndex, size_t textureIndex, size_t bumpIndex, size_t vertexShader, size_t pixelShader) {
	if (modelIndex < 0 || modelIndex >= this->models.size()) {
		throw std::runtime_error("Model index out of range.");
	}
	else if (textureIndex < 0 || textureIndex >= this->textures.size()) {
		throw std::runtime_error("Texture index out of range.");
	}
	else if (bumpIndex < 0 || bumpIndex >= this->bumpMaps.size()) {
		throw std::runtime_error("Texture index out of range for bump texture.");
	}

	ModelData data = this->models.at(modelIndex);
	Texture *texture = this->textures.at(textureIndex);
	Texture *bumpMap = this->bumpMaps.at(bumpIndex);
	Shader* vertex = this->vertexShaders.at(vertexShader);
	Shader* pixel = this->pixelShaders.at(pixelShader);

	return this->renderer->createModel(data.vertexBuffer, data.indexBuffer, texture, bumpMap, vertex, pixel);
}

Sound * RenderingEngine::createSoundFromIndex( size_t soundIndex ) {
	// ok not really a create function but it follows the naming scheme 
	// and conventions used so far in engine
	if( soundIndex < 0 || soundIndex >= this->sounds.size() ) {
		throw std::runtime_error( "Model index out of range." );
	}

	return this->sounds.at( soundIndex );
}

void RenderingEngine::dispatchSound( SoundEvent *evt ) {
	SoundObject* obj = soundCtors->invoke(evt->soundType);
	Sound* snd = obj->sound;
	if( evt->shouldStop ) {
		snd->stop();
	} else {
		if( evt->isLooped ) {
			snd->playLooped();
		} else {
			snd->play();
		}
	}

	delete obj;
	delete evt;
}

void RenderingEngine::waitForServer() {
	this->comms->waitAnnouce();
}