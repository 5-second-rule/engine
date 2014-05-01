#pragma once

/* note complete */

template< class T >
class Iterator
{
	class Node
	{
		friend class Iterator< T >;
	private:
		T value;
		Node* next;
	};

private:
	Node* atual;

	Iterator(Node*);
public:
	Iterator();
	~Iterator();
	//Iterator< T >& operator++();
	//T operator*();
};

template< class T >
Iterator< T >::Iterator(){
	atual = nullptr;
}

template< class T >
Iterator< T >::Iterator(Node* n){
	atual = n;
}

/* todo

template< class T >
Iterator< T >& operator++(){
	atual = atual->next;
	return Iterator(atual);
}


template< class T >
T operator*(){
	atual->value;
}
*/
