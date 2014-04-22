#include "Handle.h"

Handle::Handle() : Handle(0, 0, HandleType::LOCAL)
{
}


Handle::Handle(int index, int id, HandleType type)
{
	this->index = index;
	this->id = id;

	if (type == HandleType::LOCAL) {
		this->id |= LOCAL_MASK;
	}
}


Handle::~Handle()
{
}

HandleType Handle::getType() {
	return (HandleType)(this->id & LOCAL_MASK != LOCAL_MASK);
}
