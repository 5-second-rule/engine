#pragma once

#include "engine-core.h"
#include "EventType.h"
#include "Handle.h"

class COREDLL Event
{
	friend class EventDispatcher;
private:
	static const double SmallestDelay;
	EventType type;
	Handle sender;
	Handle receiver;
	double dispatchTime;
	void *extraInfo;
public:
	Event(double time,
		Handle &sender,
		Handle &receiver,
		EventType type,
		void* extraInfo);
	~Event();

	void setDispatchTime(double time);
	double getDispatchTime() const;
	Handle &getReceiver();
	bool operator<(Event const&) const;
	bool operator==(Event const&) const;
};