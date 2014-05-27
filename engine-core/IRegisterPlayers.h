#pragma once

#include "engine-core.h"
#include "BaseObject.h"

class COREDLL IRegisterPlayers {
public:
	virtual BaseObject * addPlayer(unsigned int playerGuid) = 0;

};