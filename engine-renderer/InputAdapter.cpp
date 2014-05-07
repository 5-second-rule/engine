#include "InputAdapter.h"

InputAdapter::InputAdapter() { }

Transmission::Input::KeyState InputAdapter::getKeyState(Transmission::Input::Key key) {
	return this->input->getKeyState(key);
}

void InputAdapter::setInput(Transmission::Input *input) {
	this->input = input;
}