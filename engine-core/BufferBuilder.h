#pragma once

#include "engine-core.h"

#include <queue>

// handles single child chaining in current form, doesn't handle multiple children paths
class COREDLL BufferBuilder
{
private:
	char * buffer;
	size_t size;

	std::queue<size_t> sizes;
	size_t offset;

	bool allocated;

public:
	BufferBuilder();
	~BufferBuilder();

	void reserve(size_t size);
	void allocate();
	void pop();

	char * getPointer();
	char * getBasePointer();

	const char * getPointer() const;
	const char * getBasePointer() const;
	size_t getSize() const;
};

