#pragma once

#include <queue>
#include <mutex>

template <class T>
class DoubleBufferedQueue
{
private:
	std::queue<T> *read;
	std::queue<T> *write;
	std::mutex swapMutex;

public:
	DoubleBufferedQueue() {
		this->read = new std::queue<T>();
		this->write = new std::queue<T>();
	};

	~DoubleBufferedQueue() {
		this->swapMutex.lock();

		delete this->read;
		delete this->write;

		this->swapMutex.unlock();
	};

	void push(T item) {
		this->swapMutex.lock();

		this->write->push(item);

		this->swapMutex.unlock();
	};

	bool readEmpty() {
		return this->read->empty();
	}

	T pop() {
		T item = this->read->front();
		this->read->pop();
		return item;
	}

	void swap() {
		this->swapMutex.lock();

		std::queue<T> *temp = this->read;
		this->read = this->write;
		this->write = temp;

		this->swapMutex.unlock();
	};
};

