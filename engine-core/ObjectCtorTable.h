#pragma once

#include "IHasWorldObject.h"
#include "engine-core.h"

typedef IHasWorldObject* (*ihwo_ctor)();

class COREDLL ObjectCtorTable
{
private:
	int size;
	ihwo_ctor *ctors;

public:
	ObjectCtorTable(int size);
	~ObjectCtorTable();

	void setCtor(int index, ihwo_ctor ctor);
	IHasWorldObject* invoke(int index);
};

