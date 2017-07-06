#include "Sprite.h"
#include "Tile.h"
#include "Dice.h"

#include <time.h>
#include <stdlib.h>


Sprite::Sprite()
{
}

Sprite::Sprite(int posX, int posY, bool spriteSolid, Tile tileOn, int HP, int level)
{
	_posX = posX;
	_posY = posY;
	_spriteSolid = spriteSolid;
	_tileOn = tileOn;
	_HP = HP;
	_level = level;
}

Sprite::~Sprite()
{
}

void Sprite::getSpritePos(int &x, int &y)
{
	x = _posX;
	y = _posY;
}

void Sprite::setSpritePos(int newX, int newY)
{
	_posX = newX; 
	_posY = newY; // ^ doesn't save new values!?
	int x = 0;
	x = x + 1;


 }

char Sprite::getSpriteChar()
{

	return _spriteChar;
}

void Sprite::setSpriteChar()
{
}

bool Sprite::getSpriteSolid()
{

	return _spriteSolid;
}

Tile Sprite::getTileOn()
{

	return _tileOn;
}

void Sprite::setTileOn(Tile tileOn)
{
	_tileOn = tileOn;
}

int Sprite::getLevel()
{

	return _level;
}

void Sprite::setLevel(int level)
{
	_level = level;
}

int Sprite::getExp()
{

	return _exp;
}

void Sprite::setExp(int exp)
{
	_exp = exp;
}

int Sprite::getHP()
{
	return _HP;
}

void Sprite::setHP(int damageResult)
{
	_HP = damageResult;
}

void Sprite::rollDice(int &damage)
{
	int minRange = 0, maxRange = 0, num = 0;//gets the minimum dice roll, maximum dice roll and number of dice the sprite has
	_dice.getDice(minRange, maxRange, num);

	for (int i = 0; i < num; i++) {
		damage = damage + rand() % maxRange + minRange;
	}
}

void Sprite::getDice(int &numDice, int &minRange, int &maxRange)
{
	_dice.getDice(numDice, minRange, maxRange);
}

void Sprite::setDice(int numDice, int minRange, int maxRange)
{
	_dice.setDice(numDice, minRange, maxRange);
}

int Sprite::getStarCount()
{

	return _starCount;
}

void Sprite::setStarCount(int starCount)
{
	_starCount = starCount;
}