#pragma once

#include "ISerializable.h"

class COREDLL BufferRemainder : public ISerializable
{
public:
	char *base;
	int size;
	BufferRemainder();
	~BufferRemainder();

	virtual void dehydrate(BufferBuilder *buffer);
	virtual void rehydrate(BufferBuilder *buffer);
};

