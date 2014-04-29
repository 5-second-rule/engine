#include "Handle.h"

Handle::Handle() : Handle(0, 0, HandleType::LOCAL)
{
}


Handle::Handle(int index, unsigned int id, HandleType type)
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

HandleType Handle::getType() const {
	return (HandleType)((this->id & LOCAL_MASK) != LOCAL_MASK);
}

bool Handle::operator==(Handle const& handle) const{
	if (this->id == handle.id){
		assert(this->index == handle.index);
		return true;
	}
	return false;
}