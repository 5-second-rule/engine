#pragma once

#include <vector>

#include "engine-core.h"
#include "GCHandleVector.h"
#include "Handle.h"
#include "IHasHandle.h"
#include "IUpdatable.h"
#include "ISerializable.h"
#include "ICollidable.h"
#include "CommsProcessor.h"
#include "Event.h"

#include "ConstructorTable.h"

class IHasHandle;
class CommsProcessor;
template <typename IUpdatable> class GCHandleVector;
template <typename ISerializable> class GCHandleVector;

template class COREDLL std::vector< Handle >;
template class COREDLL std::vector< IHasHandle* >;

class COREDLL World : public IUpdatable {
private:
	static const int DEFAULT_OBJECT_ALLOC = 10000;

	int lastAllocatedIndex[2];
	int objectIds[2];

	std::vector<IHasHandle *> objects[2];
	GCHandleVector<IUpdatable>* updatable;
	GCHandleVector<ISerializable>* serializable;
	GCHandleVector<ICollidable>* collidable;

public:
	World();
	~World();

	const std::vector<IHasHandle *>* getObjects() const;

	void allocateHandle(IHasHandle *object, HandleType type);
	virtual void insert(IHasHandle *object);
	void remove(Handle *handle);
	IHasHandle * get(const Handle& handle);
	void replace( const Handle& handle, IHasHandle* object );
	virtual void garbageCollectWorld();

	void printWorld();

	void broadcastUpdates(CommsProcessor *comms);

	// IUpdateable Methods
	virtual void update( float dt );

	void dispatchEvent(Event *evt, Handle &handle);
};
