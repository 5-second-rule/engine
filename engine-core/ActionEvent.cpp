#include "ActionEvent.h"


ActionEvent::ActionEvent(unsigned int playerGuid, int actionType)
	: Event(ActionEvent::TYPE)
	, actionType(actionType)
	, playerGuid(playerGuid)
{}


ActionEvent::~ActionEvent() {}


void ActionEvent::reserveSize( IReserve& buffer ) const {
	Event::reserveSize( buffer );
	buffer.reserve( sizeof( ActionHeader ) );
}

void ActionEvent::fillBuffer( IFill& buffer ) const {
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

int ActionEvent::getActionType() const {
	return this->actionType;
}

int ActionEvent::getActionType(BufferReader& reader) {
	//OH GOD THE HACKS
	return *reinterpret_cast<const int*>(reader.getPointer() + sizeof(EventHeader));
}

int ActionEvent::getPlayerGuid() const {
	return this->playerGuid;
}

