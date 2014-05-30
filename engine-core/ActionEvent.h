#pragma once
#include "Event.h"

struct COREDLL ActionHeader {
	int actionType;
	unsigned int playerGuid;
};

class COREDLL ActionEvent : public Event {
private:
	int actionType;
	int playerGuid;

	ActionEvent(); //intentionally private

public:
	ActionEvent(unsigned int playerGuid,  int actionType);
	virtual ~ActionEvent();

	int getPlayerGuid();

	// Typing
	static const EventType TYPE = EventType::ACTION;

	int getActionType() const;
	static int getActionType(BufferReader& reader);

	template<class T> static T* cast(ActionEvent* e) {
		if (static_cast<int>(T::ACTIONTYPE) == e->getActionType()) return static_cast<T*>(e);
		else return nullptr;
	}

	template<class T> static T* cast(const ActionEvent* e) {
		if (static_cast<int>(T::ACTIONTYPE) == e->getActionType()) return static_cast<const T*>(e);
		else return nullptr;
	}


	// ISerialize Methods
	virtual void reserveSize(IReserve& buffer) const;
	virtual void fillBuffer(IFill& buffer) const;
	virtual void deserialize(BufferReader& buffer);
};