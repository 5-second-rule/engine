#pragma once

#include "engine-core.h"

class COREDLL IUpdatable {
public:
	virtual void update(int dt) = 0;
};