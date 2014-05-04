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
	this->input = (Input*) this->window->getInput();
	this->renderer = Renderer::createRenderer(this->window);
	this->renderableWorld = world;
}

RenderingEngine::~RenderingEngine() {
}

void RenderingEngine::translateInput() {
	Input::KeyStateQueue queue = this->input->getInputQueue();
	Input::Key key;
	Input::KeyState state;
	Event* inputEvent;

	while (!queue.empty() && this->delegate != nullptr) {
		key = queue.front().first;
		state = queue.front().second;
		inputEvent = this->renderingDelegate->inputTranslator(key, state);
		if (inputEvent != nullptr) {
			this->sendOutboundEvent(inputEvent);
		}		
		queue.pop();
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
}

void RenderingEngine::frame(float dt) {
	renderer->clearFrame();
	this->renderableWorld->renderAll();
	renderer->drawFrame();

	this->translateInput();
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
