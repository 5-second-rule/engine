#pragma once

#include "engine-renderer.h"
#include "renderer/Input.h"
#include "../../common/common/Vector4.h"

class RENDERDLL InputAdapter
{
private:
	const Transmission::Input *input;

public:
	InputAdapter();
	void setInput(const Transmission::Input *input);
	Transmission::Input::KeyState getKeyState(Transmission::Input::Key key);
	float getAnalogTriggerValue(Transmission::Input::Key key);
	std::pair<Common::Vector4, float> getAnalogStickPosition(Transmission::Input::Key key);
	bool isControllerConnected();
};

