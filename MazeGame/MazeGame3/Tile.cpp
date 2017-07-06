#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(char tileChar, bool tileSolid, bool visited, int distanceFromObject)
{
	_tileChar = tileChar;
	_tileSolid = tileSolid;
	_visited = visited;
	_distanceFromObject = distanceFromObject;
}

Tile::~Tile()
{
}


char Tile::getTileChar()
{

	return _tileChar;
}

bool Tile::getTileSolid()
{

	return _tileSolid;
}

void Tile::setTileSolid(bool tileSolid)
{
	_tileSolid = tileSolid;
}

bool Tile::getVisited()
{

	return _visited;
}

void Tile::setVisited(bool visited)
{
	_visited = visited;
}

int Tile::getDistanceFromObject()
{

	return _distanceFromObject;
}
void Tile::setDistanceFromObject(int distanceFromObject)
{
	_distanceFromObject = distanceFromObject;
}
