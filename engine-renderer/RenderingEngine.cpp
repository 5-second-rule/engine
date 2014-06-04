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
	char* defaultPixel,
	const wchar_t* windowName
	) 
		: Engine(world, objectCtors, actionCtors, CommsProcessorRole::CLIENT) {
	this->window = Window::createWindow(appHandle, windowName, 800, 600);
	this->input = this->window->getInput();
	this->renderer = Renderer::createRenderer(this->window, defaultVertex, defaultPixel);
	this->renderer->getTimer()->StartTimer();
	this->sounder = Sounder::createSounder( this->window );
	this->renderableWorld = world;
	this->inputAdapter = InputAdapter();
	this->inputAdapter.setInput(this->input);
	this->cameraHandler = cameraHandler;
	this->soundCtors = soundCtors;
	this->renderingDelegate = nullptr;
}

RenderingEngine::~RenderingEngine() {
	for( size_t i = 0; i < textures.size(); ++i ) {
		delete this->textures[i];
	}

	for( size_t i = 0; i < bumpMaps.size(); ++i ) {
		delete this->bumpMaps[i];
	}

	for( size_t i = 0; i < pixelShaders.size(); ++i ) {
		delete this->pixelShaders[i];
	}

	for( size_t i = 0; i < vertexShaders.size(); ++i ) {
		delete this->vertexShaders[i];
	}

	for( size_t i = 0; i < sounds.size(); ++i ) {
		delete this->sounds[i];
	}

	for( size_t i = 0; i < models.size(); ++i ) {
		delete this->models[i].indexBuffer;
		delete this->models[i].vertexBuffer;
	}
	
	delete this->renderer;
	delete this->sounder;
	delete this->cameraHandler;
	delete this->soundCtors;
	delete this->window;
}

void RenderingEngine::translateInput() {
	if (this->renderingDelegate == nullptr)
		return;
	std::vector<Event *> inputEventVector = this->renderingDelegate->inputTranslator(&this->inputAdapter);
	
	std::vector<Event *>::iterator it;
	for (it = inputEventVector.begin(); it != inputEventVector.end(); ++it) {
		this->comms->sendEvent(*it);
		delete *it;
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

	// cleanup world after gc status has been pushed by server
	this->world->garbageCollectWorld();

	this->translateInput();
}

void RenderingEngine::frame(float dt) {
	// HACK to only player 0
	if (this->localPlayers.size() > 0 && this->playerMap[this->localPlayers[0]] != nullptr) {
		IHasHandle *playerObject = this->world->get(this->playerMap[this->localPlayers[0]]->cameraTarget());
		if (playerObject != nullptr) {
			this->cameraHandler->updateFor(playerObject);
			Camera *camera = renderer->getCamera();
			camera->set(this->cameraHandler->position, this->cameraHandler->lookAt, this->cameraHandler->up);
		}
	}

	renderer->clearFrame();
	if (this->frameDelegate) this->frameDelegate->beforeDraw();
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

Model * RenderingEngine::create2DModelFromVertices(Vertex *v, int numVertices, Index *i, int numIndices, Texture *texture) {
	return this->renderer->create2DModelFromVertices(v, numVertices, i, numIndices, texture);
}

Model * RenderingEngine::create2DModelFromVertices(Vertex *v, int numVertices, Index *i, int numIndices, Texture *texture, Shader *vs, Shader *ps) {
	return this->renderer->create2DModelFromVertices(v, numVertices, i, numIndices, texture, vs, ps);
}

Model * RenderingEngine::create2DModelFromScratch(Vertex *v, int numVertices, Index *i, int numIndices, char *textureFile, std::vector<Transmission::Texture *> textureStorage, bool isTransparent) {
	Texture *texture = this->renderer->createTextureFromFile(textureFile);
	if (v == nullptr || i == nullptr || texture == nullptr) {
		return nullptr;
	}

	textureStorage.push_back(texture);

	return this->renderer->create2DModelFromVertices(v, numVertices, i, numIndices, texture, isTransparent);
}
Model * RenderingEngine::create2DModelFromScratch(Vertex *v, int numVertices, Index *i, int numIndices, char *textureFile, std::vector<Transmission::Texture *> textureStorage, Shader *vs, Shader *ps, bool isTransparent) {
	Texture *texture = this->renderer->createTextureFromFile(textureFile);
	if (v == nullptr || i == nullptr || texture == nullptr || vs == nullptr || ps == nullptr) {
		return nullptr;
	}

	textureStorage.push_back(texture);

	return this->renderer->create2DModelFromVertices(v, numVertices, i, numIndices, texture, vs, ps, isTransparent);
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

	Vector4 sndPosition = Vector( evt->position[0], evt->position[1], evt->position[2] );
	float distance = (sndPosition - this->cameraHandler->position).length();
	SoundObject* obj = soundCtors->invoke(evt->soundType);
	Sound* snd = obj->sound;
	if( evt->shouldStop ) {
		snd->stop();
	} else {
		if( evt->isLooped ) {
			snd->setVolume( 1500 );
			snd->playLooped();
		} else {
			snd->setVolume( static_cast<int>(distance)*2 );
			snd->play();
		}
	}

	delete obj;
	delete evt;
}

void RenderingEngine::waitForServer() {
	this->comms->waitAnnouce();
}

void RenderingEngine::setLightBuffers(Common::Vector4 lightPos[], Common::Vector4 lightCol[], size_t num) {
	this->renderer->setLightBuffers(lightPos, lightCol, num);
}

bool RenderingEngine::shouldProcessEvents() {
	return this->frameDelegate->loadingDone();
}