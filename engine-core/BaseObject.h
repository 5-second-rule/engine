#pragma once

#include "World.h"
#include "IHasHandle.h"
#include "Handle.h"
#include "ISerializable.h"
#include "IUpdatable.h"
#include "IEventReceiver.h"

#include <queue>

class COREDLL BaseObject
	: public IHasHandle
	, public ISerializable
	, public IUpdatable
	, public IEventReceiver
{
private:
	Handle handle;

	BaseObject(); // intentionally private

protected:
	std::queue<Event *> waitingEvents;

	virtual bool handleEvent(Event *evt) = 0;

public:
	BaseObject(int type);
	virtual ~BaseObject();

	// IHasHandle Methods
	Handle getHandle();
	void setHandle(Handle handle);

	// ISerializable Methods
	virtual void reserveSize(IReserve& buffer);
	virtual void fillBuffer(IFill& buffer);
	virtual void deserialize(BufferReader& buffer);

	// IUpdateable Methods
	virtual void update(float dt);

	virtual void enqueue(Event *evt);
};