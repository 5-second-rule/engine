#include "PlayerCameraHandler.h"


PlayerCameraHandler::PlayerCameraHandler()
{
	this->position = Common::Point(0,0,-10);
	this->lookAt = Common::Point(0,0,0);
}

PlayerCameraHandler::~PlayerCameraHandler()
{
}
