#pragma once
#include "Event.h"
class COREDLL SoundEvent : public Event {
public:
	static const EventType TYPE = EventType::SOUND;

	SoundEvent();
	SoundEvent(int soundType, bool isLooped, bool shouldStop);
	~SoundEvent();

	int soundType;
	bool isLooped;
	bool shouldStop;

	//ISerializable methods
	void reserveSize( IReserve& buffer ) const;
	void fillBuffer( IFill& buffer ) const;
	void deserialize( BufferReader& reader );
protected:
};

