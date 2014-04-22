#include "RenderableObject.h"


RenderableObject::RenderableObject(Model *model)
{
	this->model = model;
}


RenderableObject::~RenderableObject()
{
	delete this->model;
}

Handle RenderableObject::getHandle() {
	return this->handle;
}

void RenderableObject::setHandle(Handle handle){
	this->handle = handle;
};

void RenderableObject::render() {
	this->model->draw();
};
