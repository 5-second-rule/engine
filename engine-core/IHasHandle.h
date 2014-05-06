#pragma once

#include "engine-core.h"
#include "Handle.h"

class COREDLL IHasHandle {
private:
	int type;
	IHasHandle();
public:
	IHasHandle(int type) : type(type) {}

	virtual Handle getHandle() = 0;
	virtual void setHandle(Handle handle) = 0;
	// TODO evaluate
	virtual int getType() 
	{ 
		return this->type; 
	}

	template<class T>
	static T* cast(IHasHandle* h) {
		if (T::TYPE == h->getType()) return static_cast<T*>(h);
		else return nullptr;
	}

};