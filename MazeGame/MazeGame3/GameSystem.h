#pragma once
#include "Tile.h"
#include "Sprite.h"
#include "Player.h"
#include "Monster.h"
#include "Distance.h"
#include <vector>

class GameSystem
{
public:
	GameSystem();
	~GameSystem();
	int menu();
	void loadGame();
	void saveGame(Player &player);
	void deleteSave();
	void newGame();
	void gameLoop(Player &player);
	
	Player createPlayer();
	void createMonsters(int posX, int posY);
	void createMaze();
	void createVisionMaze(Player &player);
	void drawMap();
	void GUI(Player player);

	void searchPlayer(Monster &monster, Player &player);
	void createDistanceMap(Player &player);
	int followPlayer(Monster &monster);

	void breakBlock(Player &player);

	bool playerMove(Player &player);
	void monsterMove(Player &player);
	bool checkMove(int x, int y, Sprite &sprite);
	void drawEntities(Sprite &sprite);
	void combat(Sprite &player, Monster &monster, int monsterNum);

protected:
	//game state
	bool _gameOver;
	//map of tiles
	Tile _map[100][200];
	//maps dimentions
	int _mapSizeX;
	int _mapSizeY;
	int _starCount;
	//keeps track of entites
	std::vector<Monster> Monsters;
	//keeps track of previous nodes
	std::vector<int> stackX; //move this to the function - does not need to be here
	std::vector<int> stackY; //could create a stack structure to hold these liek the queue
	std::vector<Distance> DistanceQueueList; //may not even need to declare these in here and just declare then in the function they are used?
};

