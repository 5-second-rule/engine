#pragma once
#include "engine-core.h"

#include "BufferReader.h"

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

	virtual T* invoke(BufferReader& reader) = 0;

protected:
	T* invoke(int index);
	void setConstructor(int index, Constructor ctor);

};

