#pragma once

#include "engine-core.h"
#include <cassert>

enum HandleType {
	LOCAL = 0,
	GLOBAL = 1
};

struct COREDLL Handle
{
private:
	static const unsigned int LOCAL_MASK = 1 << 31;
	
public:
	size_t index;
	unsigned int id;

	Handle();
	Handle(int index, unsigned int id, HandleType type);
	~Handle();
	HandleType getType() const;
	bool operator==(Handle const&) const;
};

