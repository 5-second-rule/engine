#include "RenderableObject.h"

using namespace Transmission;

RenderableObject::RenderableObject(int objectType, Model *model) 
		: BaseObject() {
	this->model = model;
}

RenderableObject::~RenderableObject() {
	delete this->model;
}

void RenderableObject::render() {
	this->model->setPosition(Vector4(this->position[0], this->position[1], this->position[2]));
	this->model->draw();
};
