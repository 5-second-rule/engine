#include "RenderingEngine.h"


#include <stdexcept>

// constants
static const char *testData = "This is a client update!!!\n";

RenderingEngine::RenderingEngine(
	RenderableWorld *world,
	ConstructorTable<IHasHandle> *objectCtors,
	ConstructorTable<ActionEvent> *actionCtors,
	void *appHandle
) 
		: Engine(world, objectCtors, actionCtors, CommsProcessorRole::CLIENT)
{
	this->window = Window::createWindow(appHandle);
	this->input = (Input*) this->window->getInput();
	this->renderer = Renderer::createRenderer(this->window);
	this->renderer->getTimer()->StartTimer();
	this->renderableWorld = world;
	this->inputAdapter = InputAdapter();
	this->inputAdapter.setInput(this->input);
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
	renderer->clearFrame();
	this->renderableWorld->renderAll();
	renderer->drawFrame();

}

int RenderingEngine::loadModel(char *filename) {
	ModelData data;

	if (this->renderer->loadModelFile(
		filename, 
		&data.vertexBuffer, 
		&data.indexBuffer)) {

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


void RenderingEngine::waitForServer() {
	this->comms->waitAnnouce();
}