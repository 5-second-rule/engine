#include "RegistrationEvent.h"


RegistrationEvent::RegistrationEvent() : Event( RegistrationEvent::TYPE ) {}


RegistrationEvent::~RegistrationEvent() {}


// ISerializable Methods
void RegistrationEvent::reserveSize( IReserve& buffer ) const {
	Event::reserveSize( buffer );
	buffer.reserve( sizeof( this->regType ) );
	buffer.reserve( sizeof( this->playerGuid ) );
	buffer.reserve( sizeof( this->response ) );
	buffer.reserve( sizeof( this->responseTag ) );
	this->objectHandle.reserveSize(buffer);
}

void RegistrationEvent::fillBuffer( IFill& buffer ) const {
	Event::fillBuffer( buffer );
	RegistrationType* type = reinterpret_cast<RegistrationType*>(buffer.getPointer());
	*type = this->regType;
	buffer.filled();
	unsigned int* playerGuid = reinterpret_cast<unsigned int*>(buffer.getPointer());
	*playerGuid = this->playerGuid;
	buffer.filled();
	Response* response = reinterpret_cast<Response*>(buffer.getPointer());
	*response = this->response;
	buffer.filled();
	int* responseTag = reinterpret_cast<int*>(buffer.getPointer());
	*responseTag = this->responseTag;
	buffer.filled();
	this->objectHandle.fillBuffer(buffer);
}

void RegistrationEvent::deserialize( BufferReader& reader ) {
	Event::deserialize( reader );
	const RegistrationType* type = reinterpret_cast<const RegistrationType*>(reader.getPointer());
	this->regType = *type;
	reader.finished( sizeof( this->regType ) );
	const unsigned int* playerGuid = reinterpret_cast<const unsigned int*>(reader.getPointer());
	this->playerGuid = *playerGuid;
	reader.finished( sizeof( this->playerGuid ) );
	const Response* response = reinterpret_cast<const Response*>(reader.getPointer());
	this->response = *response;
	reader.finished( sizeof( this->response ) );
	const int* responseTag = reinterpret_cast<const int*>(reader.getPointer());
	this->responseTag = *responseTag;
	reader.finished( sizeof( this->responseTag ) );
	this->objectHandle.deserialize(reader);
}
