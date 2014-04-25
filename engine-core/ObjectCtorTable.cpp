#include <stdexcept> 
#include "ObjectCtorTable.h"

ObjectCtorTable::ObjectCtorTable(int size) {
	this->size = size;
	this->ctors = new ihashandle_ctor[size];
	memset(this->ctors, 0, size * sizeof(ihashandle_ctor));
}

ObjectCtorTable::~ObjectCtorTable() {
	delete this->ctors;
}

void ObjectCtorTable::setCtor(int index, ihashandle_ctor ctor) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to set ctor outside of bounds.");
	}

	this->ctors[index] = ctor;
}

IHasHandle* ObjectCtorTable::invoke(int index) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to invoke ctor outside of bounds.");
	}

	ihashandle_ctor ctor = this->ctors[index];

	if (ctor == nullptr) {
		throw std::runtime_error("Attempt to invoke unset ctor.");
	}

	return ctor(this);
}
