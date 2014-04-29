#include "RenderingEngineInstance.h"

#include <stdexcept>

using namespace Transmission;

// constants
static const char *testData = "This is a client update!!!\n";

RenderingEngineInstance::RenderingEngineInstance(
	RenderableWorld *world,
	ObjectCtorTable *objectCtors,
	void *appHandle) 
		: EngineInstance(world, objectCtors, CommsProcessorRole::CLIENT) {
	
	this->window = Window::createWindow(appHandle);
	this->input = (Input*) this->window->getInput();
	this->renderer = Renderer::createRenderer(this->window);
	this->renderableWorld = world;
}

RenderingEngineInstance::~RenderingEngineInstance() {
}

void RenderingEngineInstance::handleInput() {
	Input::KeyStateQueue queue = this->input->getInputQueue();
	Input::Key key;
	Input::KeyState state;

	while (!queue.empty() && this->inputHandler != NULL) {
		key = queue.front.first();
		state = queue.front.second();
		this->sendOutboundEvent(inputHandler(key, state));
		queue.pop();
	}
}

bool RenderingEngineInstance::shouldContinueFrames() {
	Window::MessageType t;
	while ((t = this->window->getMessage()) != Window::MessageType::None) {
		if (t == Window::MessageType::Quit) return false;
	}

	return true;
}

void RenderingEngineInstance::frame(int dt) {
	this->processNetworkUpdates();
	this->world->update(dt);

	renderer->clearFrame();
	this->renderableWorld->renderAll();
	renderer->drawFrame();

	// TEST
	static int updates = 0;
	updates++;
	if (updates >= 200) {
		comms->sendUpdates(testData, strlen(testData));
		updates = 0;
	}
}

int RenderingEngineInstance::loadModel(char *filename) {
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

int RenderingEngineInstance::loadTexture(char *filename) {
	Texture *texture = this->renderer->createTextureFromFile(filename);

	// TODO validate this error check
	if (texture != nullptr) {
		this->textures.push_back(texture);
		return this->textures.size() - 1;
	}

	return -1;
}

Model * RenderingEngineInstance::createModelFromIndex(size_t modelIndex, size_t textureIndex) {
	if (modelIndex < 0 || modelIndex >= this->models.size()) {
		throw std::runtime_error("Model index out of range.");
	} else if (textureIndex < 0 || textureIndex >= this->textures.size()) {
		throw std::runtime_error("Texture index out of range.");
	}

	ModelData data = this->models.at(modelIndex);
	Texture *texture = this->textures.at(textureIndex);
	return this->renderer->createModel(data.vertexBuffer, data.indexBuffer, texture);
}

void RenderingEngineInstance::setInputHandler(RenderingEngineInstance::InputHandler handler) {
	this->inputHandler = handler;
}