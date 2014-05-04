#pragma once
#include "Event.h"
class COREDLL ActionEvent : public Event {
public:
	int actionType;
	int playerGuid;

	ActionEvent(unsigned int playerGuid );
	~ActionEvent();


	virtual void reserveSize( BufferBuilder *buffer );
	virtual void fillBuffer( BufferBuilder *buffer );
	virtual void deserialize( BufferReader& buffer );
};

