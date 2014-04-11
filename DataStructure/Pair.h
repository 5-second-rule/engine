#pragma once

template< class T, class U >
class Pair
{
private:
	Pair(T, U);
public:
	T first;
	U second;

	~Pair();
	bool operator==(const Pair) const;
	static Pair make_pair(T, U);
};

template< class T, class U >
Pair< T, U >::Pair(T f, U s){
	first = f;
	second = s;
}

template< class T, class U >
Pair< T, U >::~Pair(){

}

template< class T, class U >
bool Pair< T, U >::operator==(const Pair< T, U > p) const{
	return (this->first == p.first && this->second == p.second);
}

template< class T, class U >
Pair< T, U > Pair< T, U >::make_pair(T f, U s){
	return Pair(f, s);
}




