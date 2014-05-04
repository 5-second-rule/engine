#pragma once

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
	: public IHasHandle, 
		public ISerializable, 
		public IUpdatable,
		public IEventReceiver {
private:
	Handle handle;
	int objectType;
	float position[3];
	float force[3];
	std::queue<Event *> waitingEvents;

public:
	BaseObject(int objectType);
	virtual ~BaseObject();
	Handle getHandle();
	void setHandle(Handle handle);
	virtual int getType();

	// ISerializable Methods
	virtual void reserveSize(BufferBuilder *buffer);
	virtual void fillBuffer(BufferBuilder *buffer);
	virtual void deserialize(BufferReader& buffer);

	// IUpdateable Methods
	virtual void update(float dt);

	virtual void enqueue(Event *evt);
};