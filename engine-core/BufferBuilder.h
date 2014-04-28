#pragma once

#include "engine-core.h"

#include <stack>

// handles single child chaining in current form, doesn't handle multiple children paths
class COREDLL BufferBuilder
{
private:
	char * buffer;
	int size;

	std::stack<int> sizes;
	int offset;
	
	bool reading;

public:
	BufferBuilder();
	~BufferBuilder();

	void reserve(int size);
	void allocate();
	void pop();
	char * getPointer();
	char * getBasePointer();
	int getSize();

	static BufferBuilder * forReading(char * buffer, int size);
};

