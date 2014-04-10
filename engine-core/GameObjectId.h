#pragma once
class GameObjectId
{
private:
	static int counter;
	int id;
public:
	GameObjectId();
	GameObjectId(int);
	virtual ~GameObjectId();
	GameObjectId operator=(GameObjectId);
	bool operator==(GameObjectId);

	static GameObjectId assignUniqueObjectId();
};

