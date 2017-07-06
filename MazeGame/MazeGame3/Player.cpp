#include "Player.h"
#include "Dice.h"


Player::Player(int posX, int posY, char playerChar, bool playerSolid, Tile tileOn, int level, int HP, int numDice,
	int minRange, int maxRange, int mazeLevel, int killCount, int exp, int starCount)
{
	_posX = posX;
	_posY = posY;
	_spriteChar = playerChar;
	_spriteSolid = playerSolid;
	// not sure why i created this _tileChar
	_tileOn = tileOn;
	_level = level;
	_HP = HP;
	// need to work out dice values _dice 
	_dice.setDice(minRange, maxRange, numDice);
	_mazeLevel = mazeLevel;
	_killCount = killCount;
	_exp = exp;
	_starCount = starCount;
}


Player::~Player()
{
}


int Player::getMazeLevel()
{

	return _mazeLevel;
}

void Player::setMazeLevel()
{
}

int Player::getKillCount()
{

	return _killCount;
}

void Player::setKillCount()
{
}

int Player::getPlayerDirection()
{

	return _playerDirection;
}

void Player::setPlayerDriection(int playerDirection)
{
	_playerDirection = playerDirection;
}