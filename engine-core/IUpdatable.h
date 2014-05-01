#pragma once

#include "engine-core.h"

class COREDLL IUpdatable {
public:
	virtual void update(float dt) = 0;
};