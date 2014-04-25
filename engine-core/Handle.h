#pragma once

#include "engine-core.h"
#include <cassert>

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
	unsigned int id;

	Handle();
	Handle(int index, unsigned int id, HandleType type);
	~Handle();
	HandleType getType();
	bool operator==(Handle const&) const;
};
