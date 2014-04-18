#pragma once

#include "DoublyLinkedList.h"

template< class T >
class Stack
{
private:
	DoublyLinkedList< T > list;
public:
	Stack();
	void push(T);
	T pop();
	T top();
	int size();	
	void clear();
	~Stack();
};

template< class T >
Stack< T >::Stack(){
}

template< class T >
Stack< T >::~Stack(){
	list.clear();
}

template< class T >
void Stack< T >::push(T e){
	list.push_back(e);
}

template< class T >
T Stack< T >::pop(){
	T tmp = top();
	list.pop_back();
	return tmp;
}

template< class T >
T Stack< T >::top(){
	return list.back();
}

template< class T >
int Stack< T >::size(){
	return list.size();
}

template< class T >
void Stack< T >::clear(){
	list.clear();
}


