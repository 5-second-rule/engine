#pragma once

#include "engine-core.h"
#include "EventType.h"
#include "Handle.h"
#include <chrono>

template COREDLL class std::chrono::time_point< std::chrono::system_clock, std::chrono::system_clock::duration >;
template COREDLL class std::chrono::time_point< std::chrono::high_resolution_clock, std::chrono::high_resolution_clock::duration >;

using namespace std::chrono;
class COREDLL Event
{
private:
	EventType type;
	Handle sender;
	Handle receiver;
	high_resolution_clock::time_point dispatchTime;
	void *extraInfo;
public:
	Event();
	Event(Handle &sender,
		Handle &receiver,
		EventType type,
		void* extraInfo);
	~Event();

	void setDispatchTime(high_resolution_clock::time_point &time);
	high_resolution_clock::time_point getDispatchTime() const;
	Handle &getReceiver();
	bool operator<(Event const&) const;
	bool operator==(Event const&) const;
};