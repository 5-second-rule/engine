#pragma once
#include "Pair.h"

template< class T >
class BinaryTree
{
	class Node{
		friend class BinaryTree< T >;
	private:
		Node* left;
		Node* right;
		Node* parent;
		T keyValue;
		int balanceFactor;
		Node();
	};

private:
	Node* root;
	int elemCounter;

	void erase(Node*);
	Pair< T, bool > insert(T const&, Node*);
	T* find(T, Node*);
	bool remove(T const&, Node*);
	T removeMax(Node*);
	T removeMin(Node*);
public:
	BinaryTree();
	~BinaryTree();
	int size();
	void erase();
	Pair< T, bool > insert(T const&);
	T* find(T);
	bool remove(T const&);
};

template< class T >
BinaryTree< T >::Node::Node(){
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	balanceFactor = 0;
}

template< class T >
BinaryTree< T >::BinaryTree(){
	root = nullptr;
	elemCounter = 0;
}

template< class T >
BinaryTree< T >::~BinaryTree(){
	this->erase();
}

template< class T >
int BinaryTree< T >::size(){
	return elemCounter;
}

template< class T >
void BinaryTree< T >::erase(){
	this->erase(root);
	root = nullptr;
}

template< class T >
void BinaryTree< T >::erase(Node* n){
	if (n != nullptr){
		this->erase(n->left);
		this->erase(n->right);
		delete n;
		elemCounter--;
	}
}

/*
Insert an element in the tree return a pair with
the element inserted and a bool that is false if
the element was already in the tree.
*/
template< class T >
Pair< T, bool > BinaryTree< T >::insert(T const &e){
	if (root == nullptr){
		root = new Node();
		root->keyValue = e;
		elemCounter++;
		return Pair< T, bool>::make_pair(e, true);
	}
	return this->insert(e, root);
}

template< class T >
Pair< T, bool > BinaryTree< T >::insert(T const &e, Node* n){
	if (e > n->keyValue){
		if (n->right == nullptr){
			n->right = new Node();
			n->right->keyValue = e;
			n->right->parent = n;
			elemCounter++;
			return Pair< T, bool >::make_pair(e, true);
		}
		else {
			return insert(e, n->right);
		}
	}
	else if (e < n->keyValue){
		if (n->left == nullptr){
			n->left = new Node();
			n->left->keyValue = e;
			n->left->parent = n;
			elemCounter++;
			return Pair< T, bool >::make_pair(e, true);
		}
		else {
			return insert(e, n->left);
		}
	}
	else {
		int tmp = n->keyValue;
		n->keyValue = e;
		return Pair< T, bool >::make_pair(tmp, false);
	}
}

template< class T >
bool BinaryTree< T >::remove(T const &e){
	return remove(e, root);
}

template< class T >
bool BinaryTree< T >::remove(T const &e, Node *n){
	if (n == nullptr)
		return false;
	if (e < n->keyValue){
		return remove(e, n->left);
	}
	else if (e > n->keyValue){
		return remove(e, n->right);
	}
	else {
		if (n->left)
			n->keyValue = removeMax(n->left);
		else if (n->right)
			n->keyValue = removeMin(n->right);
		else {
			if (n->parent){
				if (n->parent->left == n)
					n->parent->left = nullptr;
				else
					n->parent->right = nullptr;
			}
			elemCounter--;
			delete n;
		}
		return true;
	}
}

template< class T >
T BinaryTree< T >::removeMax(Node *n){
	T tmp;
	if (n->right)
		removeMax(n->right);
	if (n->parent){
		if (n->parent->left == n)
			n->parent->left = n->left;
		else
			n->parent->right = n->left;
	}
	tmp = n->keyValue;
	elemCounter--;
	delete n;
	return tmp;
}

template< class T >
T BinaryTree< T >::removeMin(Node *n){
	T tmp;
	if (n->left)
		removeMin(n->left);
	if (n->parent){
		if (n->parent->left == n)
			n->parent->left = n->right;
		else
			n->parent->right = n->right;
	}
	tmp = n->keyValue;
	elemCounter--;
	delete n;
	return tmp;
}

template< class T >
T* BinaryTree< T >::find(T e){
	return this->find(e, root);
}

template< class T >
T* BinaryTree< T >::find(T e, Node* n){
	if (n == nullptr)
		return nullptr;
	if (e < n->keyValue){
		return find(e, n->left);
	}
	else if (e > n->keyValue){
		return find(e, n->right);
	}
	else {
		return &(n->keyValue);
	}
}