#include "Dice.h"

Dice::Dice()
{
}


Dice::~Dice()
{
}

void Dice::setDice(int minRange, int maxRange, int num)
{
	_minRange = minRange;
	_maxRange = maxRange;
	_num = num;
}

void Dice::getDice(int &minRange, int &maxRange, int &num)
{
	minRange = _minRange;
	maxRange = _maxRange;
	num = _num;
}