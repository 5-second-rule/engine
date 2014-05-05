#pragma once
#include "Event.h"
class COREDLL ActionEvent : public Event {
public:
	int actionType;
	int playerGuid;

	ActionEvent(unsigned int playerGuid );
	~ActionEvent();


	virtual void reserveSize( IReserve& buffer );
	virtual void fillBuffer( IFill& buffer );
	virtual void deserialize( BufferReader& buffer );
};

