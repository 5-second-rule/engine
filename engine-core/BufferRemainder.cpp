#include "BufferRemainder.h"


BufferRemainder::BufferRemainder() {
	this->size = 0;
	this->base = nullptr;
}


BufferRemainder::~BufferRemainder() {
	if (this->base != nullptr) {
		delete[] this->base;
	}
}

void BufferRemainder::reserveSize(IReserve& buffer) {
	// does nothing lol
}

void BufferRemainder::fillBuffer(IFill& buffer) {
	// does nothing lol
}

void BufferRemainder::deserialize(BufferReader& buffer) {

	this->size = buffer.getRemainingSize();
	this->base = new char[this->size];

	memcpy(this->base, buffer.getPointer(), this->size);
}