#pragma once
#include "engine-core.h"

enum COREDLL EventType{
	Event_Attack = 0,
	Event_Move = 1
};

class COREDLL EventAttack{
	int damage;
};

class COREDLL EventMove{
	int blablabla;
};