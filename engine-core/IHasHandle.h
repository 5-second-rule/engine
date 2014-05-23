#pragma once

#include "engine-core.h"
#include "Handle.h"
#include "ISerializable.h"

class COREDLL IHasHandle {
private:
	int type;
	Handle handle;

	IHasHandle();

protected:
	void setType(int type) {
		this->type = type;
	}

public:
	IHasHandle(int type) : type(type) {}

	Handle getHandle(){
		return this->handle;
	}

	void setHandle(Handle handle){
		this->handle = handle;
	}

	virtual std::string toString(){
		return handle.toString();
	}

	virtual int getType() const { 
		return this->type; 
	}

	template<class T>
	static T* cast(IHasHandle* h) {
		if (T::TYPE == h->getType()) return static_cast<T*>(h);
		else return nullptr;
	}

};