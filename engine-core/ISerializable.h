#pragma once

#include "engine-core.h"
#include "BufferBuilder.h"

class COREDLL ISerializable {
public:
	// TODO decide on real signatures, etc
	virtual void dehydrate(BufferBuilder *buffer) = 0;
	virtual void rehydrate(BufferBuilder *buffer) = 0;
};