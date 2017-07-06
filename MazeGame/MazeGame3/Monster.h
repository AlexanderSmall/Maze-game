#pragma once
#include "Sprite.h"
#include "Player.h"

class Monster: public Sprite
{
public:
	Monster();
	Monster(int posX, int posY, char monsterChar, bool monsterSolid, Tile tileOn, int level, int HP, int numDice, int minRange, int maxRange,
		bool playerFound, int movementSpeed, int movementCount);
	~Monster();

	bool getPlayerFound();
	void setPlayerFound(bool playerFound);
	int getMovementSpeed();
	int getMovementCount();
	void setMovementCount(int movementCount);

protected:
	bool _playerFound;
	int _movementSpeed;
	int _movementCount;
};

