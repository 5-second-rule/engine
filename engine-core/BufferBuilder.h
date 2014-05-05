#pragma once

#include "engine-core.h"

#include <queue>

class COREDLL IReserve {
public:
	virtual void reserve(size_t size) = 0;
};

class COREDLL IFill {
public:
	virtual char * getPointer() = 0;
	virtual void filled() = 0; //TODO - rename
};

// handles single child chaining in current form, doesn't handle multiple children paths
class COREDLL BufferBuilder : public IReserve, public IFill
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
	void filled();

	char * getPointer();
	char * getBasePointer();

	const char * getPointer() const;
	const char * getBasePointer() const;
	size_t getSize() const;
};

