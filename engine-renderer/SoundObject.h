#pragma once

#include "engine-renderer.h"
#include "renderer\Sound.h"
#include "engine-core\ISerializable.h"
class RENDERDLL SoundObject : public ISerializable {
public:
	SoundObject( Transmission::Sound* sound);
	~SoundObject();

	Transmission::Sound* sound;

	//ISerializable methods
	void reserveSize( IReserve& buffer ) const;
	void fillBuffer( IFill& buffer ) const;
	void deserialize( BufferReader& reader );

	static int getType( BufferReader& );
};