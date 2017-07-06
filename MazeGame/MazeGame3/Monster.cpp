#include "Monster.h"
#include "Sprite.h"
#include "Tile.h"


Monster::Monster()
{
}

Monster::Monster(int posX, int posY, char monsterChar, bool monsterSolid, Tile tileOn, int level, int HP, int numDice,
	int minRange, int maxRange, bool playerFound, int movementSpeed, int movementCount)
{
	_posX = posX;
	_posY = posY;
	_spriteChar = monsterChar;
	_spriteSolid = monsterSolid;
	_tileOn = tileOn;
	_level = level;
	_HP = HP;
	//sort dice roll mech
	_dice.setDice(minRange, maxRange, numDice);
	_playerFound = playerFound;
	_movementSpeed = movementSpeed;
	_movementCount = movementCount;
}


Monster::~Monster()
{
}


bool Monster::getPlayerFound()
{

	return _playerFound;
}

void Monster::setPlayerFound(bool playerFound)
{
	_playerFound = playerFound;
}

int Monster::getMovementSpeed()
{
	return _movementSpeed;
}

int Monster::getMovementCount()
{
	return _movementCount;
}

void Monster::setMovementCount(int movementCount)
{
	_movementCount = movementCount;
}