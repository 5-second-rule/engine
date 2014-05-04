#pragma once
#include "engine-core.h"

class COREDLL BufferReader
{
private:
	const char * buffer;
	const size_t size;
	size_t offset;

public:
	BufferReader(char *buffer, size_t size);
	~BufferReader();

	void finished(size_t size);
	size_t getRemainingSize() const;
	const char * getPointer() const;
};

