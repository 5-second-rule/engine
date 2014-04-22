#pragma once

#include "engine-core.h"

enum HandleType {
	LOCAL = 0,
	GLOBAL = 1
};

class COREDLL Handle
{
private:
	static const unsigned int LOCAL_MASK = 1 << 31;
	
public:
	int index;
	int id;

	Handle();
	Handle(int index, int id, HandleType type);
	~Handle();
	HandleType getType();
};

