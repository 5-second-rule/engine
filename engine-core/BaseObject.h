#pragma once

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
	virtual void setGC();
	void setHandle(Handle handle);

	// ISerializable Methods
	virtual void reserveSize(IReserve& buffer) const;
	virtual void fillBuffer(IFill& buffer) const;
	virtual void deserialize(BufferReader& buffer);

	// IUpdateable Methods
	virtual void update(float dt);

	virtual void enqueue(Event *evt);

	int getType() const {
		return IHasHandle::getType();
	}

	static int getType(BufferReader&);

	std::string toString() const;
};