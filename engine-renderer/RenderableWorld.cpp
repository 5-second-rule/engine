#include "RenderableWorld.h"


RenderableWorld::RenderableWorld()
{
}


RenderableWorld::~RenderableWorld()
{
}

void RenderableWorld::insert(IHasHandle *object) {
	World::insert(object);

	Handle handle = object->getHandle();
	if (dynamic_cast<IRenderable*>(object) != nullptr) {
		this->renderable.push_back(handle);
	}
}

void RenderableWorld::renderAll() {
	auto iterator = this->renderable.begin();
	while (iterator != this->renderable.end()) {
		IRenderable *renderable = dynamic_cast<IRenderable *>(this->get(*iterator));

		if (renderable != nullptr) {
			renderable->render();
		}

		iterator++;
	}
}