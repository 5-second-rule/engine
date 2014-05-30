#include "BufferRemainder.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

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