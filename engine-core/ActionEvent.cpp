#include "ActionEvent.h"


ActionEvent::ActionEvent(unsigned int playerGuid, int actionType)
	: Event(EventType::ACTION)
	, actionType(actionType)
	, playerGuid(playerGuid)
{}


ActionEvent::~ActionEvent() {}


void ActionEvent::reserveSize( IReserve& buffer ) {
	Event::reserveSize( buffer );
	buffer.reserve( sizeof( ActionHeader ) );
}

void ActionEvent::fillBuffer( IFill& buffer ) {
	Event::fillBuffer( buffer );
	struct ActionHeader *actionHdr = reinterpret_cast<struct ActionHeader *>(buffer.getPointer());

	actionHdr->actionType = this->actionType;
	actionHdr->playerGuid = this->playerGuid;
	buffer.filled();
}


void ActionEvent::deserialize( BufferReader& buffer ) {
	Event::deserialize( buffer );
	const struct ActionHeader *actionHdr = reinterpret_cast<const struct ActionHeader *>(buffer.getPointer());
	this->actionType = actionHdr->actionType;
	this->playerGuid = actionHdr->playerGuid;
	buffer.finished( sizeof( struct ActionHeader ) );
}

int ActionEvent::getActionType() {
	return this->actionType;
}

int ActionEvent::getActionType(BufferReader& reader) {
	//TODO
}

int ActionEvent::getPlayerGuid() {
	return this->playerGuid;
}

