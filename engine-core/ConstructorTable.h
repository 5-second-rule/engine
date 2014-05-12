#pragma once
#include "engine-core.h"

#include <stdexcept> 

#include "BufferReader.h"
#include "Event.h"

template<typename T>
class ConstructorTable
{
	typedef T* (*Constructor)(ConstructorTable<T> *table, Args* args);

private:
	int size;
	Constructor *ctors;

public:
	ConstructorTable(int size);
	virtual ~ConstructorTable();

	T* invoke(int index, Args* args);

protected:
	void setConstructor(int index, Constructor ctor);

};

// templates means we have to define everything in the header
// why? I don't know, he's on third, and I DON'T GIVE A DAMN!
// Oh, he's our shortstop.

template<typename T>
ConstructorTable<T>::ConstructorTable(int size) {
	this->size = size;
	this->ctors = new Constructor[size];
	memset(this->ctors, 0, size * sizeof(Constructor));
}

template<typename T>
ConstructorTable<T>::~ConstructorTable() {
	delete [] this->ctors;
}

template<typename T>
void ConstructorTable<T>::setConstructor(int index, Constructor ctor) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to set ctor outside of bounds.");
	}

	this->ctors[index] = ctor;
}

template<typename T>
T* ConstructorTable<T>::invoke(int index, Args* args) {
	if (index < 0 || index >= size) {
		throw std::runtime_error("Attempt to invoke ctor outside of bounds.");
	}

	Constructor ctor = this->ctors[index];

	if (ctor == nullptr) {
		throw std::runtime_error("Attempt to invoke unset ctor.");
	}

	return ctor(this, args);
}

