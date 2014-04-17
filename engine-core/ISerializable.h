#pragma once

#include "engine-core.h"

class COREDLL ISerializable {
public:
	// TODO decide on real signatures, etc
	virtual void dehydrate() = 0;
	virtual void rehydrate() = 0;
};