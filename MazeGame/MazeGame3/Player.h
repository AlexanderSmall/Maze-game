#pragma once
#include "Sprite.h"

class Player: public Sprite //remove inheritance and make all monsters and players the same.
{
public:
	Player(int posX, int posY, char playerChar, bool playerSolid, Tile tileOn, int level, int HP, int numDice, int minRange,
		int maxRange, int mazeLevel, int killCount, int exp, int starCount);
	~Player();

	int getMazeLevel();
	void setMazeLevel();
	int getKillCount();
	void setKillCount();
	int getPlayerDirection();
	void setPlayerDriection(int playerDirection);
protected:
	int _mazeLevel;
	int _killCount;
	int _playerDirection;
};
