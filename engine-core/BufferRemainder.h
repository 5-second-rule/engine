#pragma once

#include "ISerializable.h"

class COREDLL BufferRemainder : public ISerializable
{
public:
	char *base;
	int size;
	BufferRemainder();
	~BufferRemainder();

	virtual void reserveSize(IReserve& buffer);
	virtual void fillBuffer(IFill& buffer);
	virtual void deserialize(BufferReader& buffer);
};

