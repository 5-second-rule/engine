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