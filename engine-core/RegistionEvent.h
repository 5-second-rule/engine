#pragma once
#include "Event.h"
#include "Handle.h"
#include "ISerializable.h"
#include "EventType.h"

enum class RegistrationType {
	REQUEST,
	RESPONSE
}; 

enum class COREDLL Response {
	OK,
	FAIL
};

class RegistionEvent : public Event {
public:
	static const EventType TYPE = EventType::REGISTRATION;
	RegistrationType regType;
	unsigned int playerGuid;
	int responseTag;
	Response response;
	
	RegistionEvent();
	~RegistionEvent();

	//ISerializable methods
	void reserveSize( IReserve& buffer ) const;
	void fillBuffer( IFill& buffer ) const;
	void deserialize( BufferReader& reader );
};

