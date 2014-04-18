#pragma once

#include "BinaryTree.h"
#include "Pair.h"

template< class keyT, class valueT >
class MapNode{
public:
	bool operator<(MapNode const&) const;
	bool operator>(MapNode const&) const;
	bool operator<=(MapNode const&) const;
	bool operator>=(MapNode const&) const;
	bool operator==(MapNode const&) const;

	~MapNode();
	MapNode();
	MapNode(keyT);
	MapNode(keyT, valueT);
	valueT value;
private:
	keyT key;
};

template< class keyT, class valueT >
class Map
{
	friend class MapNode< keyT, valueT >;

	BinaryTree< MapNode< keyT, valueT > > tree;
public:
	valueT operator[](keyT);
	void insert(Pair< keyT, valueT >);
	void insert(keyT, valueT);
	int size();
	Map();
	~Map();
	void clear();
};

template< class keyT, class valueT >
Map< keyT, valueT >::Map(){
}

template< class keyT, class valueT >
Map< keyT, valueT >::~Map(){
	clear();
}

template< class keyT, class valueT >
MapNode< keyT, valueT >::MapNode(keyT key, valueT value){
	this->value = value;
	this->key = key;
}

template< class keyT, class valueT >
MapNode< keyT, valueT >::MapNode(keyT key){
	this->key = key;
}

template< class keyT, class valueT >
MapNode< keyT, valueT >::MapNode(){
}

template< class keyT, class valueT >
MapNode< keyT, valueT >::~MapNode(){
}

template< class keyT, class valueT >
bool MapNode< keyT, valueT >::operator<(MapNode const &n) const{
	return this->key < n.key;
}

template< class keyT, class valueT >
bool MapNode< keyT, valueT >::operator>(MapNode const &n) const{
	return this->key > n.key;
}

template< class keyT, class valueT >
bool MapNode< keyT, valueT >::operator<=(MapNode const &n) const{
	return this->key <= n.key;
}

template< class keyT, class valueT >
bool MapNode< keyT, valueT >::operator>=(MapNode const &n) const{
	return this->key >= n.key;
}

template< class keyT, class valueT >
bool MapNode< keyT, valueT >::operator==(MapNode const &n) const{
	return this->key == n.key;
}

template< class keyT, class valueT >
valueT Map< keyT, valueT >::operator[](keyT key){
	MapNode< keyT, valueT > *n = tree.find(MapNode< keyT, valueT >(key));

	if (n == nullptr)
		throw "Element not found in the map";
	
	return n->value;
}

template< class keyT, class valueT >
void Map< keyT, valueT >::insert(Pair< keyT, valueT > p){
	tree.insert(MapNode< keyT, valueT >(p.first, p.second));
}

template< class keyT, class valueT >
void Map< keyT, valueT >::insert(keyT key, valueT value){
	tree.insert(MapNode< keyT, valueT >(key, value));
}

template< class keyT, class valueT >
int Map< keyT, valueT >::size(){
	return tree.size();
}

template< class keyT, class valueT >
void Map< keyT, valueT >::clear(){
	tree.clear();
}