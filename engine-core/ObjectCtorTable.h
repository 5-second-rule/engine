#pragma once

#include "IHasWorldObject.h"

typedef IHasWorldObject* (*ihwo_ctor)();

class ObjectCtorTable
{
private:
	int size;
	ihwo_ctor *ctors;

public:
	ObjectCtorTable(int size);
	~ObjectCtorTable();

	void setCtor(ihwo_ctor ctor, int index);
	IHasWorldObject* invoke(int index);
};

