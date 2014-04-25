#pragma once

#include "engine-core.h"
#include "IHasHandle.h"

typedef IHasHandle* (*ihashandle_ctor)(void *thisObj);

class COREDLL ObjectCtorTable
{
private:
	int size;
	ihashandle_ctor *ctors;

public:
	ObjectCtorTable(int size);
	~ObjectCtorTable();

	void setCtor(int index, ihashandle_ctor ctor);
	IHasHandle* invoke(int index);
};

