#pragma once

#include "engine-core.h"
#include "BufferBuilder.h"
#include "BufferReader.h"

class COREDLL ISerializable {
public:
	// TODO decide on real signatures, etc
	virtual void reserveSize(IReserve& buffer) const = 0;
	virtual void fillBuffer(IFill& buffer) const = 0;

	virtual void deserialize(BufferReader& reader) = 0;

	void serialize(BufferBuilder& buffer) const {
		this->reserveSize(buffer);
		buffer.allocate();
		this->fillBuffer(buffer);
	}
};