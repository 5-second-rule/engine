#pragma once

#include <vector>

#include "engine-core.h"
#include "Handle.h"
#include "IHasHandle.h"
#include "IUpdatable.h"
#include "ISerializable.h"
#include "CommsProcessor.h"
#include "Event.h"

class IHasHandle;

using namespace std;
template class COREDLL vector< Handle >;
template class COREDLL vector< IHasHandle* >;

class COREDLL World : public IUpdatable {
private:
	static const int DEFAULT_OBJECT_ALLOC = 10000;

	int lastAllocatedIndex[2];
	int objectIds[2];

	std::vector<IHasHandle *> objects[2];
	std::vector<Handle> updatable;
	std::vector<Handle> serializable;
	long int frameCounter;
public:
	World();
	~World();

	
	void allocateHandle(IHasHandle *object, HandleType type);
	virtual void insert(IHasHandle *object);
	void remove(Handle *handle);
	IHasHandle * get(const Handle *handle);

	void broadcastUpdates(CommsProcessor *comms);

	// IUpdateable Methods
	virtual void update( float dt );

	void dispatchEvent(Event *evt, Handle &handle);
	bool isTick(long int n); // Return true if the number of frames already updated is multiple of n
};
