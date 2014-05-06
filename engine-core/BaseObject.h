#pragma once

#include "World.h"
#include "IHasHandle.h"
#include "Handle.h"
#include "ISerializable.h"
#include "IUpdatable.h"
#include "IEventReceiver.h"

#include <queue>

struct BaseObjectInfo {
	/*	Array of bytes contains tagged data, one after another
	------------------------------------------------------------------------
	| index | id | position[3] | velocity[3] | accleration[3] | force[3]
	------------------------------------------------------------------------
	*/
	int index;
	unsigned int id;
	int objectType;
	float position[3];
	float force[3];
};

class COREDLL BaseObject
	: public IHasHandle
	, public ISerializable
	, public IUpdatable
	, public IEventReceiver
{
private:
	Handle handle;
	int objectType;
	float force[3];

protected:
	float position[3];
	std::queue<Event *> waitingEvents;

	virtual bool handleEvent(Event *evt);

public:
	BaseObject();
	virtual ~BaseObject();

	// IHasHandle Methods
	Handle getHandle();
	void setHandle(Handle handle);

	virtual int getType();

	// ISerializable Methods
	virtual void reserveSize(IReserve& buffer);
	virtual void fillBuffer(IFill& buffer);
	virtual void deserialize(BufferReader& buffer);

	// IUpdateable Methods
	virtual void update(float dt);

	virtual void enqueue(Event *evt);
};