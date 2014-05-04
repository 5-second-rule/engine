#include "ActionEvent.h"


ActionEvent::ActionEvent(unsigned int playerGuid, size_t index) : playerGuid(playerGuid), index(index) {
	this->type = static_cast<int>(EventType::ACTION);
}


ActionEvent::~ActionEvent() {}


void ActionEvent::reserveSize( BufferBuilder *buffer ) {
	Event::reserveSize( buffer );
	buffer->reserve( sizeof( struct ActionHeader ) );
}

void ActionEvent::fillBuffer( BufferBuilder *buffer ) {
	Event::fillBuffer( buffer );
	struct ActionHeader *actionHdr = reinterpret_cast<struct ActionHeader *>(buffer->getPointer());

	actionHdr->actionType = this->actionType;
	actionHdr->index = this->index;
	actionHdr->playerGuid = this->playerGuid;
	buffer->pop();
}


void ActionEvent::deserialize( BufferReader& buffer ) {
	Event::deserialize( buffer );
	const struct ActionHeader *actionHdr = reinterpret_cast<const struct ActionHeader *>(buffer.getPointer());
	this->actionType = actionHdr->actionType;
	this->index = actionHdr->index;
	this->playerGuid = actionHdr->playerGuid;
	buffer.finished( sizeof( struct ActionHeader ) );
}
