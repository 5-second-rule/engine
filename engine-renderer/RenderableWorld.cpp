#include "RenderableWorld.h"


RenderableWorld::RenderableWorld(){
	this->renderable = new GCHandleVector<IRenderable>(this);
}

RenderableWorld::~RenderableWorld() {
	delete this->renderable;
}


void RenderableWorld::insert(IHasHandle *object) {
	World::insert(object);

	Handle handle = object->getHandle();
	if( dynamic_cast<IRenderable*>(object) != nullptr ) {
		this->renderable->push_back( handle );
	}
}

void RenderableWorld::renderAll() {

	for( int i = 0; i < this->renderable->size(); i++ ) {
		IRenderable *u = this->renderable->getIndirect( i, false );

		if( u != nullptr ) {
			u->render();
		}
	}
}

void RenderableWorld::garbageCollectWorld() {
	World::garbageCollectWorld();
	this->renderable->collectGarbage();
}