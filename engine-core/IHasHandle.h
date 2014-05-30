#pragma once

#include "engine-core.h"
#include "Handle.h"
#include "ISerializable.h"

class COREDLL IHasHandle {
private:
	int type;
	IHasHandle();

protected:
	void setType(int type) {
		this->type = type;
	}

public:
	IHasHandle(int type) : type(type) {}

	virtual Handle getHandle() = 0;
	virtual void setHandle(Handle handle) = 0;

	virtual int getType() const { 
		return this->type; 
	}

	template<class T>
	static T* cast(IHasHandle* h) {
		if (T::TYPE == h->getType()) return static_cast<T*>(h);
		else return nullptr;
	}

	virtual std::string toString() {
		// default implementation, should be overridden
		// to print more useful info.
		return this->getHandle().toString();
	}
};