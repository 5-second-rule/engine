#pragma once

#include "engine-core.h"
#include <cassert>

#include "ISerializable.h"

enum HandleType {
	LOCAL = 0,
	GLOBAL = 1
};

class COREDLL Handle : public ISerializable
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

	void reserveSize(IReserve& buffer);
	void fillBuffer(IFill& buffer);
	void deserialize(BufferReader& buffer);


};

