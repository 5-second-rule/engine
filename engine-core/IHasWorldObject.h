#pragma once

#include "WorldObject.h"
#include "engine-core.h"

class COREDLL IHasWorldObject {
public:
	WorldObject* getWorldObject();
};