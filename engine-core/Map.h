#pragma once

template< class K, class V >
class Map
{
private:
	int elements_counter;
public:
	Map();
	virtual ~Map();
	V operator[](K key);
	int size();
	void clear();
};

template< class K, class V >
Map< K, V >::Map(){
	elements_counter = 0;
}

template< class K, class V >
V Map< K, V >::operator[](K key){

}

template< class K, class V >
int Map< K, V >::size(){
	return elements_counter;
}

