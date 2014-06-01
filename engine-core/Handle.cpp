#include "Handle.h"
#include <sstream>

Handle::Handle() : Handle(0, 0, HandleType::LOCAL) {}


Handle::Handle(int index, unsigned int id, HandleType type) {
	this->index = index;
	this->id = id;

	if (type == HandleType::LOCAL) {
		this->id |= LOCAL_MASK;
	}
}

Handle::~Handle() {}

HandleType Handle::getType() const {
	return (HandleType)((this->id & LOCAL_MASK) != LOCAL_MASK);
}

bool Handle::isLocal() const {
	return getType() == HandleType::LOCAL;
}

bool Handle::operator==(Handle const& handle) const {
	if (this->id == handle.id){
		assert(this->index == handle.index);
		return true;
	}
	return false;
}

void Handle::reserveSize(IReserve& buffer) const {
	buffer.reserve(sizeof(size_t) + sizeof(unsigned int));
}

void Handle::fillBuffer(IFill& buffer) const {
	char *buf = buffer.getPointer();
	memcpy(buf, &this->index, sizeof(size_t));
	memcpy(buf + sizeof(size_t), &this->id, sizeof(unsigned int));
	buffer.filled();
}

void Handle::deserialize(BufferReader& reader) {
	const char* buf = reader.getPointer();

	memcpy(&this->index, buf, sizeof(size_t));
	memcpy(&this->id, buf + sizeof(size_t), sizeof(unsigned int));

	reader.finished(sizeof(size_t) + sizeof(unsigned int));
}

std::string Handle::toString() const {
	std::stringstream buffer;
	buffer << "---Handle---" << std::endl;
	buffer << "index: " << index << std::endl;
	buffer << "id: " << id;
	return buffer.str();
}