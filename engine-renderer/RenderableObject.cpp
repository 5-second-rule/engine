#include "RenderableObject.h"

using namespace Transmission;

RenderableObject::RenderableObject(Model* model) : model(model) {}

RenderableObject::~RenderableObject() {
	delete this->model;
}

void RenderableObject::render() {
	this->model->draw();
};

IMoveable* RenderableObject::getMoveable() {
	return this->model;
}
