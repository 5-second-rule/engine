#include "RenderableObject.h"

using namespace Transmission;

RenderableObject::RenderableObject(Model* model) : model(model) {}

RenderableObject::~RenderableObject() {
	delete this->model;
}

void RenderableObject::render() {
	this->model->draw();
};

void RenderableObject::setPosition(const float pos[3]) {
	this->model->setPosition(Common::Point(pos[0],pos[1],pos[2]));
}

void RenderableObject::setRotation(const Vector4& rot) {
	this->model->setRotation(rot.x(),rot.y(),rot.z());
}
