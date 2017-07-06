#pragma once

class Tile
{
public:
	Tile();
	Tile(char tileChar, bool tileSolid, bool visited, int distanceFromObject);
	~Tile();

	char getTileChar();
	bool getTileSolid();
	void setTileSolid(bool tileSolid);
	bool getVisited();
	void setVisited(bool visited);
	int getDistanceFromObject();
	void setDistanceFromObject(int distanceFromObject);
protected:
	char _tileChar;
	bool _tileSolid;
	bool _visited;
	int _distanceFromObject;
};

