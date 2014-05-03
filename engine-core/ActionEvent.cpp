#include "ActionEvent.h"


ActionEvent::ActionEvent(int actionType, unsigned int playerGuid, size_t index) {
	this->actionType = actionType;
	this->playerGuid = playerGuid;
	this->index = index;
}


ActionEvent::~ActionEvent() {}
