#pragma once
#include "Event.h"
class COREDLL ActionEvent : public Event {
public:
	int playerGuid;

	// this should go away once we get GUID id-ing working
	size_t index;

	ActionEvent(unsigned int playerGuid, size_t index);
	~ActionEvent();
};

