#pragma once
#include "engine-core.h"
#include "Handle.h"

enum class COREDLL EventType{
	OBJECT_UPDATE,
	ACTION,
	SPECIAL
};

struct COREDLL ObjectUpdateHeader {
	Handle handle;
	int objectType;
};

struct COREDLL ActionHeader {
	int actionType;
	unsigned int playerGuid;
	
	// this should go away once we get GUID id-ing working
	size_t index;
};