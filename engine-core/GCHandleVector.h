#pragma once

#include <vector>
#include <algorithm>

#include "World.h"
#include "Handle.h"
#include "IHasHandle.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class World;

// Garbage collecting handle vector
template <typename TSpecific>
class GCHandleVector
{
private:
	World *world;
	std::vector<Handle>* handles;
	std::vector<int>* deadIndexes;
public:
	GCHandleVector(World *world) {
		this->handles = new std::vector<Handle>();
		this->deadIndexes = new std::vector<int>();
		this->world = world;
	}

	~GCHandleVector() {
		delete this->handles;
		delete this->deadIndexes;
	}

	int size() {
		return this->handles->size();
	}

	void reserve(int capacity) {
		this->handles->reserve(capacity);
	}

	void push_back(Handle &handle) {
		this->handles->push_back(handle);
	}

	// Not sure on perf of get 1, 2, 3 over iterator
	TSpecific * getIndirect(int index, bool isGCSweep, IHasHandle **result) {
		*result = this->world->get(this->handles->at(index));

		if (*result == nullptr && isGCSweep) {
			this->deadIndexes->push_back(index);
		}

		return dynamic_cast<TSpecific *>(*result);
	}

	TSpecific * getIndirect(int index, bool isGCSweep) {
		IHasHandle *result;
		return this->getIndirect(index, isGCSweep, &result);
	}

	// not very good runtime.
	// there are linear ways to do it by copying to new vector, but not convenient to implement at first
	void collectGarbage() {
		if (this->deadIndexes->size() > 0) {
			sort(this->deadIndexes->begin(), this->deadIndexes->end());

			// delete backwards to preserve forwards indexes until removal
			for (int i = this->deadIndexes->size() - 1; i >= 0; i--) {
				this->handles->erase(this->handles->begin() + this->deadIndexes->at(i));
			}

			this->deadIndexes->clear();
		}
	}
};

