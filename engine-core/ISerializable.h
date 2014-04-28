#pragma once

#include "engine-core.h"

class COREDLL ISerializable {
public:
	// TODO decide on real signatures, etc
	virtual void dehydrate(char *dst, size_t &size, size_t dstSize) = 0;
	virtual void rehydrate( char *data) = 0;
};