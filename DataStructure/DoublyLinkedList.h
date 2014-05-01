#pragma once

template< class T >
class DoublyLinkedList
{
	class Node
	{
		friend class DoublyLinkedList< T >;
	private:
		T value;
		Node *front;
		Node *back;

		Node();
		Node(T);
	};
private:
	Node *begin;
	Node *end;
	int elementCounter;

public:
	DoublyLinkedList();
	void push_back(T);
	void push_front(T);
	void pop_back();
	void pop_front();
	T back();
	T front();
	void clear();
	int size();
	~DoublyLinkedList();
};

template< class T >
DoublyLinkedList< T >::DoublyLinkedList(){
	end = new Node();
	begin = end;
	elementCounter = 0;
}

template< class T >
DoublyLinkedList< T >::Node::Node(){
	front = nullptr;
	back = nullptr;
}

template< class T >
DoublyLinkedList< T >::Node::Node(T value){
	front = nullptr;
	back = nullptr;
	this->value = value;
}

template< class T >
DoublyLinkedList< T >::~DoublyLinkedList(){
	clear();
}

template< class T >
void DoublyLinkedList< T >::push_back(T value){
	Node *tmp;
	if (begin == end){
		begin = new Node(value);
		begin->front = end;
		end->back = begin;
	}
	else {
		tmp = new Node(value);
		tmp->back = end->back;
		tmp->back->front = tmp;
		end->back = tmp;
		tmp->front = end;
	}
	elementCounter++;
}

template< class T >
void DoublyLinkedList< T >::push_front(T value){
	Node *tmp;
	if (begin == end){
		begin = new Node(value);
		begin->front = end;
		end->back = begin;
	}
	else {
		tmp = new Node(value);
		tmp->front = begin;
		begin->back = tmp;
		begin = tmp;
	}
	elementCounter++;
}

template< class T >
void DoublyLinkedList< T >::pop_back(){
	Node *tmp;
	tmp = end->back;
	if (tmp == nullptr)
		throw "Empty List";
	if (tmp == begin){
		begin = end;
	}
	else {
		tmp->back->front = end;
		end->back = tmp->back;
	}
	elementCounter--;
	delete tmp;
}

template< class T >
void DoublyLinkedList< T >::pop_front(){
	Node *tmp;
	tmp = begin;
	if (tmp == nullptr)
		throw "Empty List";
	if (tmp->front == end){
		end->back = nullptr;
		begin = end;
	}
	else {
		begin = begin->front;
		begin->back = nullptr;
	}
	elementCounter--;
	delete tmp;
}

template< class T >
T DoublyLinkedList< T >::back(){
	return end->back->value;
}

template< class T >
T DoublyLinkedList< T >::front(){
	return begin->value;
}

template< class T >
void DoublyLinkedList< T >::clear(){
	Node *tmp;
	if (begin == end)
		return;
	tmp = begin;
	while (tmp != end){
		delete tmp->back;
		tmp = tmp->front;
	}
	delete tmp->back;
	end->back = nullptr;
	begin = end;
	elementCounter = 0;
}

template< class T >
int DoublyLinkedList< T >::size(){
	return elementCounter;
}