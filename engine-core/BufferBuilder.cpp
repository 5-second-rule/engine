#include <stdexcept>

#include "BufferBuilder.h"


BufferBuilder::BufferBuilder() {
	this->buffer = nullptr;
	this->size = 0;
	this->offset = 0;
	this->allocated = false;
}

BufferBuilder::~BufferBuilder() {
	if (this->buffer != nullptr) {
		delete[] this->buffer;
	}
}

void BufferBuilder::reserve(size_t size) {
	if (this->allocated) return;

	this->sizes.push(size);
	this->size += size;
}

void BufferBuilder::filled() {
	if (!this->allocated) return;

	this->offset += this->sizes.front();
	this->sizes.pop();
}

void BufferBuilder::allocate() {
	if (this->allocated || this->buffer != nullptr) {
		throw new std::runtime_error("buffer already allocated");
	}

	this->buffer = new char[this->size];
	this->allocated = true;
}

// Accessor Methods

char * BufferBuilder::getPointer() {
	return this->buffer + this->offset;
}

char * BufferBuilder::getBasePointer() {
	return this->buffer;
}

const char * BufferBuilder::getPointer() const {
	return this->buffer + this->offset;
}

const char * BufferBuilder::getBasePointer() const {
	return this->buffer;
}

size_t BufferBuilder::getSize() const {
	return this->size;
}