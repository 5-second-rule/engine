#include "RenderingEngine.h"

#include <stdexcept>

using namespace Transmission;

// constants
static const char *testData = "This is a client update!!!\n";

RenderingEngine::RenderingEngine(
	RenderableWorld *world,
	ObjectCtorTable *objectCtors,
	void *appHandle) 
		: Engine(world, objectCtors, CommsProcessorRole::CLIENT) {
	
	this->window = Window::createWindow(appHandle);
	this->input = this->window->getInput();
	this->renderer = Renderer::createRenderer(this->window);
	this->renderableWorld = world;
	this->inputAdapter = InputAdapter();
	this->inputAdapter.setInput(this->input);
}

RenderingEngine::~RenderingEngine() {
}

void RenderingEngine::translateInput() {
	std::vector<Event *> inputEventVector = this->renderingDelegate->inputTranslator(&this->inputAdapter);
	std::vector<Event *>::iterator it;
	for (it = inputEventVector.begin(); it != inputEventVector.end(); ++it) {
		this->sendOutboundEvent(*it);
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
	Engine::tick(dt);
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


void RenderingEngine::waitForServer() {
	this->comms->waitAnnouce();
}