#pragma once
#include "engine-core.h"

#include "ActionEvent.h"
#include "Handle.h"

class COREDLL PlayerDelegate {
public:
	virtual void handleEvent(ActionEvent* evt) = 0;
	virtual Handle cameraTarget() = 0;
};

class COREDLL IRegisterPlayers {
public:
	virtual PlayerDelegate* addPlayer(unsigned int playerGuid) = 0;

};