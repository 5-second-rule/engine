#include <stdexcept> 
#include "ObjectCtorTable.h"

ObjectCtorTable::ObjectCtorTable(int size) {
	this->size = size;
	this->ctors = new ihwo_ctor[size];
	memset(this->ctors, 0, size * sizeof(ihwo_ctor));
}

ObjectCtorTable::~ObjectCtorTable() {
	delete this->ctors;
}

void ObjectCtorTable::setCtor(ihwo_ctor ctor, int index) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to set ctor outside of bounds.");
	}
}

IHasWorldObject* ObjectCtorTable::invoke(int index) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to invoke ctor outside of bounds.");
	}

	ihwo_ctor ctor = this->ctors[index];

	if (ctor == nullptr) {
		throw std::runtime_error("Attempt to invoke unset ctor.");
	}

	return ctor();
}
