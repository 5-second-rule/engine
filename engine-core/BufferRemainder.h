#pragma once

#include "ISerializable.h"

class COREDLL BufferRemainder : public ISerializable
{
public:
	char *base;
	int size;
	BufferRemainder();
	~BufferRemainder();

	virtual void reserveSize(BufferBuilder *buffer);
	virtual void fillBuffer(BufferBuilder *buffer);
	virtual void deserialize(BufferReader *buffer);
};

