#include "BufferRemainder.h"


BufferRemainder::BufferRemainder()
{
	this->size = 0;
	this->base = nullptr;
}


BufferRemainder::~BufferRemainder()
{
	if (this->base != nullptr) {
		delete[] this->base;
	}
}

void BufferRemainder::dehydrate(BufferBuilder *buffer) {
	// does nothing lol
}

void BufferRemainder::rehydrate(BufferBuilder *buffer) {
	buffer->reserve(0); // force offset update

	this->size = buffer->getSize() - (buffer->getPointer() - buffer->getBasePointer());
	this->base = new char[this->size];

	memcpy(this->base, buffer->getPointer(), this->size);
}