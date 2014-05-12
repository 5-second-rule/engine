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
		[](ConstructorTable<Event>* t, Args* args) -> Event* {
			UpdateArgs* updateArgs = static_cast<UpdateArgs*>(args);
			if( updateArgs != nullptr )
				return new UpdateEvent(updateArgs->handle, updateArgs->child);
			else
				return new UpdateEvent( updateArgs->handle, nullptr );
		}
	);
}

EventFactory::~EventFactory() {}

Event* EventFactory::invoke( EventType type, Args* args ) {
	Event* e;
	if (type == EventType::ACTION) {
		ActionArgs* actionArgs = static_cast<ActionArgs*>(args);
		e = this->actionEventCtors->invoke(actionArgs->actionType, args);
	} else {
		e = ConstructorTable<Event>::invoke(static_cast<int>(type), args);
	}
	return e;
}
