#pragma once

#include "ConstructorTable.h"
#include "ActionEvent.h"

class EventFactory : public ConstructorTable<Event>
{
private:
	ConstructorTable<ActionEvent>* actionEventCtors;
public:
	EventFactory(ConstructorTable<ActionEvent>* ctorTable);
	~EventFactory();

	virtual Event* invoke(BufferReader& reader);
};

