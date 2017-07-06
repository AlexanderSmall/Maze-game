#include "Distance.h"

Distance::Distance()
{
}


Distance::~Distance()
{
}

void Distance::popItems(int &posX, int &posY, int &distance)
{
	posX = _posX;
	posY = _posY;
	distance = _distance;
}

void Distance::pushItems(int posX, int posY, int distance)
{
	_posX = posX;
	_posY = posY;
	_distance = distance;

}