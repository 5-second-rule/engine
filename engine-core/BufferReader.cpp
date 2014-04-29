#include "BufferReader.h"


BufferReader::BufferReader(char *buffer, size_t size)
	: buffer(buffer)
	, size(size)
	, offset(0)
{}

BufferReader::~BufferReader() {}

void BufferReader::finished(size_t size) {
	this->offset += size;
}

const char * BufferReader::getPointer() const {
	return this->buffer + offset;
}

size_t BufferReader::getRemainingSize() const {
	return this->size - offset;
}


