#pragma once

#include "engine-core.h"
#include "BaseObject.h"

class COREDLL IRegisterPlayers {
protected:
	int state;
public:
	virtual BaseObject * addPlayer(unsigned int playerGuid) = 0;
	virtual int getState() {
		return this->state;
	};
};