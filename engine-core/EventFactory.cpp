#include "EventFactory.h"

#include "EventType.h"
#include "UpdateEvent.h"

#define SIZE 10

EventFactory::EventFactory(ConstructorTable<ActionEvent>* ctorTable)
	: ConstructorTable<Event>(SIZE)
	, actionEventCtors(ctorTable)
{
	this->setConstructor(
		static_cast<int>(EventType::UPDATE),
		[](ConstructorTable<Event>* t) -> Event* {
			return new UpdateEvent();
		}
	);
}

EventFactory::~EventFactory() {}

Event* EventFactory::invoke(BufferReader& reader) {
	EventType type = Event::getType(reader);

	if (type == EventType::ACTION) {
		return this->actionEventCtors->invoke(reader);
	}
	
	Event* e = ConstructorTable<Event>::invoke(static_cast<int>(type));
	e->deserialize(reader);
	return e;
}
