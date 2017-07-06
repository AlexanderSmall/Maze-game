#pragma once
class Dice
{
public:
	Dice();
	~Dice();

	void setDice(int minRange, int maxRange, int num);
	void getDice(int &minRange, int &maxRange, int &num);

protected:
	int _minRange;
	int _maxRange;
	int _num;
};

