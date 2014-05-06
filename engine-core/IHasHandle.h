#pragma once

#include "engine-core.h"
#include "Handle.h"
#include "World.h"

class World;

class COREDLL IHasHandle {
public:
	virtual Handle getHandle() = 0;
	virtual void setHandle(Handle handle) = 0;
	// TODO evaluate
	virtual int getType() 
	{ 
		return -1; 
	}
};