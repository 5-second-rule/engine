#pragma once
#include "DoublyLinkedList.h"

template< class T >
class Queue
{
private:
	DoublyLinkedList< T > list;
public:
	Queue();
	~Queue();
	T front();
	T back();
	void push_back(T);
	void pop_front();
	void clear();
	int size();
};

template< class T >
Queue< T >::Queue(){
}

template< class T >
Queue< T >::~Queue(){
	list.clear();
}

template< class T >
T Queue< T >::front(){
	return list.front();
}

template< class T >
T Queue< T >::back(){
	return list.back();
}

template< class T >
void Queue< T >::push_back(T e){
	list.push_back(e);
}

template< class T >
void Queue< T >::pop_front(){
	list.pop_front();
}

template< class T >
int Queue< T >::size(){
	return list.size();
}

template< class T >
void Queue< T >::clear(){
	list.clear();
}