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
	const struct EventHeader *hdr = reinterpret_cast<const struct EventHeader *>(buffer.getPointer());
	buffer.finished( sizeof( struct EventHeader ) );
}
