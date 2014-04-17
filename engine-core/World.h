#pragma once

#include "engine-core.h"
#include "Handle.h"
#include "IHasHandle.h"
#include "IUpdatable.h"
#include "ISerializable.h"

#include <vector>

class COREDLL World
{
private:
	static const int DEFAULT_OBJECT_ALLOC = 10000;

	int lastAllocatedIndex[2];
	int objectIds[2];

	std::vector<IHasHandle *> objects[2];
	std::vector<Handle> updatable;
	std::vector<Handle> serializable;
	
public:
	World();
	~World();

	
	void allocateHandle(IHasHandle *object, HandleType type);
	virtual void insert(IHasHandle *object);
	void remove(Handle *handle);
	IHasHandle * get(Handle *handle);
};
