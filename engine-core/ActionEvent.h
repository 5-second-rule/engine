#pragma once
#include "Event.h"
class COREDLL ActionEvent : public Event {
public:
	int actionType;
	int playerGuid;

	// this should go away once we get GUID id-ing working
	size_t index;

	ActionEvent(unsigned int playerGuid, size_t index);
	~ActionEvent();


	virtual void reserveSize( BufferBuilder *buffer );
	virtual void fillBuffer( BufferBuilder *buffer );
	virtual void deserialize( BufferReader& buffer );
};

