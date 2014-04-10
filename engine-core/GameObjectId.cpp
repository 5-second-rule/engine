#include "GameObjectId.h"

int GameObjectId::counter = 0;

GameObjectId::GameObjectId()
{
}

GameObjectId::GameObjectId(int id){
	this->id = id;
}

GameObjectId::~GameObjectId()
{
}

bool GameObjectId::operator==(GameObjectId g){
	return this->id == g.id;
}

GameObjectId GameObjectId::operator=(GameObjectId g){
	GameObjectId tmp;
	tmp.id = g.id;
	return tmp;
}

GameObjectId GameObjectId::assignUniqueObjectId(){
	return GameObjectId(++counter);
}

