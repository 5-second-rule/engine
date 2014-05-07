#pragma once

#include "ConstructorTable.h"
#include "ActionEvent.h"

class EventFactory : ConstructorTable<Event>
{
private:
	ConstructorTable<ActionEvent>* actionEventCtors;
public:
	EventFactory(ConstructorTable<ActionEvent>* ctorTable);
	~EventFactory();

	Event* invoke(BufferReader& reader);
};

