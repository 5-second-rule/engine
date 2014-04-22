#include "RenderingEngineInstance.h"

#include <stdexcept> 

RenderingEngineInstance::RenderingEngineInstance(
	RenderableWorld *world,
	ObjectCtorTable *objectCtors,
	void *appHandle) 
		: EngineInstance(world, objectCtors) {
	
	this->window = Window::createWindow(appHandle);
	this->renderer = Renderer::createRenderer(this->window);
	this->renderableWorld = world;
}

RenderingEngineInstance::~RenderingEngineInstance() {
}

bool RenderingEngineInstance::shouldContinueFrames() {
	Window::MessageType t;
	while ((t = this->window->getMessage()) != Window::MessageType::None) {
		if (t == Window::MessageType::Quit) return false;
	}

	return true;
}

void RenderingEngineInstance::frame() {
	renderer->clearFrame();
	this->renderableWorld->renderAll();
	renderer->drawFrame();
}

int RenderingEngineInstance::loadModelFile(char *filename) {
	ModelData data;

	// TODO make abstraction without unnecessary model creation
	Model * model = this->renderer->createModelFromFile(
		filename, 
		&data.vertexBuffer, 
		&data.indexBuffer);

	this->modelData.push_back(data);
	return this->modelData.size() - 1;
}

Model * RenderingEngineInstance::createModelFromIndex(int modelIndex) {
	if (modelIndex < 0 || modelIndex >= this->modelData.size()) {
		throw std::runtime_error("Model index out of range.");
	}

	ModelData data = this->modelData.at(modelIndex);
	this->renderer->createModel(data.vertexBuffer, data.indexBuffer);
}

