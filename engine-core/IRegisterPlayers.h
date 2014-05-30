#pragma once

#include "engine-core.h"
#include "BaseObject.h"

class COREDLL PlayerDelegate {
public:
	virtual Handle routeEvent(const ActionEvent* evt) = 0;
	virtual Handle cameraTarget() = 0;
};

class COREDLL IRegisterPlayers {
public:
	virtual PlayerDelegate* addPlayer(unsigned int playerGuid) = 0;

};