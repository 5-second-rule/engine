#pragma once
#include "Pair.h"



template< class T >
class AVL
{
	class Node{
		friend class AVL< T >;
	private:
		Node* left;
		Node* right;
		T key_value;
		int balance_factor;
		Node();
	};

private:
	Node* root;
	int elem_counter;

	void erase(Node*);
	Pair< T, bool > insert(T, Node*);
	T* find(T, Node*);
public:
	AVL();
	~AVL();
	int size();
	void erase();
	Pair< T, bool > insert(T);
	T* find(T);
	bool remove(T);
};

template< class T >
AVL< T >::Node::Node(){
	left = nullptr;
	right = nullptr;
	balance_factor = 0;
}

template< class T >
AVL< T >::AVL(){
	root = nullptr;
	elem_counter = 0;
}

template< class T >
AVL< T >::~AVL(){
	this->erase();
}

template< class T >
int AVL< T >::size(){
	return elem_counter;
}

template< class T >
void AVL< T >::erase(){
	this->erase(root);
	root = nullptr;
}

template< class T >
void AVL< T >::erase(Node* n){
	if (n != nullptr){
		this->erase(n->left);
		this->erase(n->right);
		delete n;
		elem_counter--;
	}
}

/*
	Insert an element in the tree return a pair with
	the element inserted and a bool that is false if
	the element was already in the tree.
*/
template< class T >
Pair< T, bool > AVL< T >::insert(T e){
	if (root == nullptr){
		root = new Node();
		root->key_value = e;
		elem_counter++;
		return Pair< T, bool>::make_pair(e, true);
	}
	return this->insert(e, root);
}

template< class T >
Pair< T, bool > AVL< T >::insert(T e, Node* n){
	if (e > n->key_value){
		if (n->right == nullptr){
			n->right = new Node();
			n->right->key_value = e;
			elem_counter++;
			return Pair< T, bool >::make_pair(e, true);
		}
		else {
			return insert(e, n->right);
		}
	}
	else if (e < n->key_value){
		if (n->left == nullptr){
			n->left = new Node();
			n->left->key_value = e;
			elem_counter++;
			return Pair< T, bool >::make_pair(e, true);
		}
		else {
			return insert(e, n->left);
		}
	}
	else {
		int tmp = n->key_value;
		n->key_value = e;
		return Pair< T, bool >::make_pair(tmp, false);
	}	
}

template< class T >
T* AVL< T >::find(T e){
	return this->find(e, root);
}

template< class T >
T* AVL< T >::find(T e, Node* n){
	if (n == nullptr)
		return nullptr;
	if (e < n->key_value){
		return find(e, n->left);
	}
	else if (e > n->key_value){
		return find(e, n->right);
	}
	else {
		return &(n->key_value);
	}
}