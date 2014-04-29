#pragma once

#include "engine-core.h"
#include "BufferBuilder.h"
#include "BufferReader.h"

class COREDLL ISerializable {
public:
	// TODO decide on real signatures, etc
	virtual void reserveSize(BufferBuilder *buffer) = 0;
	virtual void fillBuffer(BufferBuilder *buffer) = 0;

	virtual void deserialize(BufferReader *reader) = 0;

	void serialize(BufferBuilder *buffer) {
		this->reserveSize(buffer);
		buffer->allocate();
		this->fillBuffer(buffer);
	}
};