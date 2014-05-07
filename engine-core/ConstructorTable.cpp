#include <stdexcept> 
#include "ConstructorTable.h"

template<typename T>
ConstructorTable<T>::ConstructorTable(int size) {
	this->size = size;
	this->ctors = new Constructor[size];
	memset(this->ctors, nullptr, size * sizeof(Constructor));
}

template<typename T>
ConstructorTable<T>::~ConstructorTable() {
	delete[] this->ctors;
}

template<typename T>
void ConstructorTable<T>::setConstructor(int index, Constructor ctor) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to set ctor outside of bounds.");
	}

	this->ctors[index] = ctor;
}

template<typename T>
T* ConstructorTable<T>::invoke(int index) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to invoke ctor outside of bounds.");
	}

	Constructor ctor = this->ctors[index];

	if (ctor == nullptr) {
		throw std::runtime_error("Attempt to invoke unset ctor.");
	}

	return ctor(this);
}