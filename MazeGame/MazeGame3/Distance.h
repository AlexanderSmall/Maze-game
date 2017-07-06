#pragma once
class Distance
{
public:
	Distance();
	~Distance();

	void popItems(int &posX, int &posY, int &distance);
	void pushItems(int posX, int posY, int distance);

protected:
	int _posX;
	int _posY;
	int _distance;
};

