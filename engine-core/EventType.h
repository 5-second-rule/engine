#pragma once
#include "engine-core.h"
#include "Handle.h"

enum COREDLL EventType{
	OBJECT_UPDATE = 0,
	SPECIAL = 1
};

struct COREDLL ObjectUpdateHeader {
	Handle handle;
	int objectType;
};