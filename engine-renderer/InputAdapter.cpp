#include "InputAdapter.h"

InputAdapter::InputAdapter() { }

void InputAdapter::setInput(const Transmission::Input *input) {
	this->input = input;
}

Transmission::Input::KeyState InputAdapter::getKeyState(Transmission::Input::Key key) {
	return this->input->getKeyState(key);
}

float InputAdapter::getAnalogTriggerValue(Transmission::Input::Key key) {
	return this->input->getAnalogTriggerValue(key);
}

std::pair<Common::Vector4, float> InputAdapter::getAnalogStickPosition(Transmission::Input::Key key) {
	return this->input->getAnalogStickPosition(key);
}

bool InputAdapter::isControllerConnected() {
	return this->input->isControllerConnected();
}