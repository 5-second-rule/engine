#pragma once

#include "engine-renderer.h"
#include "renderer/Input.h"

class RENDERDLL InputAdapter
{
private:
	Transmission::Input *input;

public:
	InputAdapter();
	Transmission::Input::KeyState getKeyState(Transmission::Input::Key key);
	void setInput(Transmission::Input *input);
};

