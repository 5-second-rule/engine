#pragma once

enum EventType{
	EVENT_ATTACK
};

class Event
{
private:
	EventType type;

public:
	Event();
	virtual ~Event();
};

