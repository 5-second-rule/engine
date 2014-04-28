#pragma once

#include "engine-core.h"
#include "IHasHandle.h"

class ObjectCtorTable;

typedef IHasHandle* (*ihashandle_ctor)(ObjectCtorTable *thisObj);

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

