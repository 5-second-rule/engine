#pragma once
#include "Event.h"
class COREDLL ActionEvent : public Event {
private:
	int actionType;
	int playerGuid;

	ActionEvent(); //intentionally private

public:
	ActionEvent(unsigned int playerGuid,  int actionType);
	virtual ~ActionEvent();

	static const EventType TYPE = EventType::ACTION;

	virtual void reserveSize( IReserve& buffer );
	virtual void fillBuffer( IFill& buffer );
	virtual void deserialize( BufferReader& buffer );

	int getActionType();
	template<class T> static T* cast(ActionEvent* e) {
		if (static_cast<int>(T::ACTIONTYPE) == e->getActionType()) return static_cast<T*>(e);
		else return nullptr;
	}
};