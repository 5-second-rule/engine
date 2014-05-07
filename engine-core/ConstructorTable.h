#pragma once
#include "engine-core.h"

template<typename T>
class COREDLL ConstructorTable
{
	typedef T* (*Constructor)(ConstructorTable<T> *table);

private:
	int size;
	Constructor *ctors;

public:
	ConstructorTable(int size);
	virtual ~ConstructorTable();

	T* invoke(int index);

protected:
	void setConstructor(int index, Constructor ctor);

};

