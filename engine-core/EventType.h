#pragma once
#include "engine-core.h"
#include "Handle.h"

enum COREDLL EventType{
	OBJECT_UPDATE = 0
};

class COREDLL EventAttack{
	int damage;
};

class COREDLL EventMove{
	int blablabla;
};

struct COREDLL ObjectUpdateHeader {
	Handle handle;
	int objectType;
};