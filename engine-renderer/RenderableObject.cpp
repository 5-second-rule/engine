#include "RenderableObject.h"

using namespace Transmission;

RenderableObject::RenderableObject(int objectType, Model *model) 
		: BaseObject(objectType) {
	this->model = model;
}

RenderableObject::~RenderableObject() {
	delete this->model;
}

void RenderableObject::render() {
	//TODO: set position
	this->model->draw();
};
