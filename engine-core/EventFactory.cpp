#include "EventFactory.h"

#include "EventType.h"
#include "UpdateEvent.h"
#include "RegistrationEvent.h"
#include "SoundEvent.h"

#define SIZE 10

EventFactory::EventFactory(ConstructorTable<ActionEvent>* ctorTable)
	: ConstructorTable<Event>(SIZE)
	, actionEventCtors(ctorTable)
{
	this->setConstructor(
		static_cast<int>(EventType::UPDATE),
		[](ConstructorTable<Event>* t) -> Event* {
			return new UpdateEvent(Handle(), nullptr);
		}
	);

	this->setConstructor(
		static_cast<int>(EventType::REGISTRATION),
		[](ConstructorTable<Event>* t) -> Event* {
			return new RegistrationEvent();
		}
	);

	this->setConstructor(
		static_cast<int>(EventType::SOUND),
		[]( ConstructorTable<Event>* t ) -> Event* {
		return new SoundEvent();
	}
	);
}

EventFactory::~EventFactory() {}

Event* EventFactory::invoke( BufferReader& reader ) {
	Event* e;
	EventType type = Event::getType(reader);
	if (type == EventType::ACTION) {
		e = this->actionEventCtors->invoke(ActionEvent::getActionType(reader));
	} else {
		e = ConstructorTable<Event>::invoke(static_cast<int>(type));
	}

	e->deserialize(reader);

	return e;
}
