#pragma once
#include "Tile.h"
#include "Dice.h"
class Sprite
{
public:
	Sprite();
	Sprite(int posX, int posY, bool spriteSolid, Tile tileOn, int HP, int level);
	~Sprite();

	void getSpritePos(int &x, int &y);
	void setSpritePos(int newX, int newY);
	char getSpriteChar();
	void setSpriteChar();
	bool getSpriteSolid();
	Tile getTileOn();
	void setTileOn(Tile tileOn);
	int getLevel();
	void setLevel(int level);
	int getExp();
	void setExp(int exp);
	int getHP();
	void setHP(int damageResult);
	void rollDice(int &damage);
	void getDice(int &numDice, int &minRange, int &maxRange);
	void setDice(int numDice, int minRange, int maxRange);
	int getStarCount();
	void setStarCount(int starCount);
protected:
	int _posX;
	int _posY;
	char _spriteChar;
	bool _spriteSolid;
	Tile _tileOn;
	int _level;
	int _exp;
	int _HP;
	Dice _dice;
	int _starCount;
};

