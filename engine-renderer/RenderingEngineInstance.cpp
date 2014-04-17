#include "RenderingEngineInstance.h"

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