#include "SoundObject.h"


SoundObject::SoundObject( Transmission::Sound* sound ) : sound(sound) {}


SoundObject::~SoundObject() {}

void SoundObject::reserveSize( IReserve& buffer ) const {
	throw std::runtime_error( "SoundObject Should not be serialized" );
}

void SoundObject::fillBuffer( IFill& buffer ) const {
	throw std::runtime_error( "SoundObject Should not be serialized" );
}

void SoundObject::deserialize( BufferReader& buffer ) {
	throw std::runtime_error( "SoundObject Should not be serialized" );
}

int SoundObject::getType( BufferReader& buffer ) {
	throw std::runtime_error( "SoundObject Should not be serialized" );
}
