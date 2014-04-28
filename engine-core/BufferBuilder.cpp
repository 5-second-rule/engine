#include "BufferBuilder.h"

#include <stdexcept>:

BufferBuilder::BufferBuilder() {
	this->buffer = nullptr;
	this->size = 0;
	this->offset = 0;
	this->reading = false;
}

BufferBuilder::~BufferBuilder() {
}

void BufferBuilder::reserve(int size) {
	if (!this->sizes.empty()) {
		this->offset += this->sizes.top();
	}

	this->sizes.push(size);

	if (this->reading) {
		if (this->offset + size > this->size) {
			throw new std::runtime_error("buffer too small");
		}
	} else {
		this->size += size;
	}
}

void BufferBuilder::pop() {
	this->sizes.pop();

	if (!this->sizes.empty()) {
		this->offset -= this->sizes.top();
	}
}

void BufferBuilder::allocate() {
	if (this->buffer != nullptr) {
		throw new std::runtime_error("buffer already allocated");
	}

	this->buffer = new char[this->size];
}

char * BufferBuilder::getPointer() {
	return this->buffer + this->offset;
}

char * BufferBuilder::getBasePointer() {
	return this->buffer;
}

int BufferBuilder::getSize() {
	return this->size;
}

BufferBuilder * BufferBuilder::forReading(char * buffer, int size) {
	// TODO add some sanity checks

	BufferBuilder *builder = new BufferBuilder();
	builder->buffer = buffer;
	builder->size = size;
	builder->reading = true;

	return builder;
}