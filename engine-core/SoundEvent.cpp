#include "SoundEvent.h"

SoundEvent::SoundEvent() : Event( SoundEvent::TYPE ) {}

SoundEvent::SoundEvent( int soundType, bool isLooped, bool shouldStop ) :
	Event( SoundEvent::TYPE ), soundType(soundType), isLooped(isLooped), shouldStop(shouldStop) {}


SoundEvent::~SoundEvent() {}


// ISerializable Methods
void SoundEvent::reserveSize( IReserve& buffer ) const {
	Event::reserveSize( buffer );
	buffer.reserve( sizeof( this->soundType ) );
	buffer.reserve( sizeof( this->isLooped ) );
	buffer.reserve( sizeof( this->shouldStop ) );
}

void SoundEvent::fillBuffer( IFill& buffer ) const {
	Event::fillBuffer( buffer );
	int* soundType = reinterpret_cast<int*>(buffer.getPointer());
	*soundType= this->soundType;
	buffer.filled();
	bool* isLooped = reinterpret_cast<bool*>(buffer.getPointer());
	*isLooped = this->isLooped;
	buffer.filled();
	bool* shouldStop = reinterpret_cast<bool*>(buffer.getPointer());
	*shouldStop = this->shouldStop;
	buffer.filled();
}

void SoundEvent::deserialize( BufferReader& reader ) {
	Event::deserialize( reader );
	const int* soundType = reinterpret_cast<const int*>(reader.getPointer());
	this->soundType = *soundType;
	reader.finished( sizeof( this->soundType ) );
	const bool* isLooped = reinterpret_cast<const bool*>(reader.getPointer());
	this->isLooped = *isLooped;
	reader.finished( sizeof( this->isLooped ) );
	const bool* shouldStop = reinterpret_cast<const bool*>(reader.getPointer());
	this->shouldStop = *shouldStop;
	reader.finished( sizeof( this->shouldStop ) );
}