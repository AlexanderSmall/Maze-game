#include "GameSystem.h"
#include "Tile.h"
#include "Sprite.h"
#include "Player.h"
#include "Monster.h"
#include "Distance.h"
#include "Dice.h"

#include <iostream>
#include <stdlib.h>  
#include <time.h>
#include <vector>
#include <conio.h>
#include <math.h> 
#include <fstream>
#include <string>
#include <windows.h>

GameSystem::GameSystem()
{
	_mapSizeX = 31;
	//_mapSizeY = 11;
	_mapSizeY = 13; //use this for final version

	//roughly fills the whole console - should stop map growing larger than this.
	//Y biggest 25 
	//X biggest 71
	srand(time(NULL)); //seeds random num gen 
}

GameSystem::~GameSystem()
{
}

int GameSystem::menu()
{
	char keyPressed = '5';

	while ((keyPressed != '1') && (keyPressed != '2') && (keyPressed != '3')) { //error handler for any input tahts not 1 - 3
		std::cout <<
			"   ,--.   ,--.                           ,----.                              " << std::endl <<
			"   |   `.'   | ,--,--.,-----. ,---.     '  .-./    ,--,--.,--,--,--. ,---.   " << std::endl <<
			"   |  |'.'|  || ,-.  |`-.  / | .-. :    |  | .---.| ,-.  ||        || .-. :  " << std::endl <<
			"   |  |   |  || '-'  | /  `-.|   --.    '  '--'  || '-'  ||  |  |  ||   --.  " << std::endl <<
			"   `--'   `--' `--`--'`-----' `----'     `------'  `--`--'`--`--`--' `----'  " << std::endl <<
			"        (  )                                                     )    )      " << std::endl <<
			"      (; )  ,           ______________________________           (  )  ,     " << std::endl <<
			"     ,()(,)()          |                              |         ,()()(,)     " << std::endl <<
			"     () (__))          |   [1] NEW GAME               |         (),(;_)(     " << std::endl <<
			"   --{~~~^~~~}--       |   [2] LOAD GAME              |       --{~~~^~~~}--  " << std::endl <<
			"      [_~0~_]          |   [3] EXIT                   |          [_~0~_]     " << std::endl <<
			"       -|.|-           |                              |           -|.|-      " << std::endl <<
			"        |||            --------------------------------            |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl <<
			"        |||                                                        |||       " << std::endl;

		keyPressed = _getch();
		std::ifstream checkSave("MapSave.txt"); //opens the mapsave file inorder to check if there is a current saved game
		std::string checkFile;
		getline(checkSave, checkFile);
		checkSave.close();

		switch (keyPressed) {
		case '1':
			//check file to see if there is already a game created
			if (checkFile == ".") {
				_gameOver = false;
				newGame();
			}
			else {
				std::cout << "You are about to overwrite a previous save do you wish to continue? [1] yes [2] no" << std::endl;
				char overwrite = _getch();
				switch (overwrite) {
				case '1':
						_gameOver = false;
						deleteSave(); //delete sthe current save if there is one
						newGame(); break; //creates a new game
				case '2': menu(); break;//clear the screen and reprint the menu
				}
			}
			break;
		case '2':
			//open file to check if there is a saved game
			if (checkFile != ".") {
				loadGame();
			}
			else {
				std::cout << "No previously saved game" << std::endl;
				system("PAUSE");
				menu();
			}
			break;
		case '3': //nothing
			keyPressed = 'a';
			_gameOver = true;
			return 0;
			break;
		}
	}

}

void GameSystem::loadGame()
{
	std::ifstream loadMap("MapSave.txt");
	std::string gameSystemSetUp;
	std::string value;
	int count = 0;
	int mapX, mapY, starCount;

	getline(loadMap, gameSystemSetUp);

	for (int i = 0; i < gameSystemSetUp.size(); i++) { //reads the map attributes
		if (gameSystemSetUp[i] != ',') { //skips the commas 
			value = value + gameSystemSetUp[i]; //value is equal to the data in the file
		}
		else {
			count++;
			switch (count) {
			case 1: mapX = atoi(value.c_str()); break;
			case 2: mapY = atoi(value.c_str()); break;
			case 3: starCount = atoi(value.c_str()); break;
			}
			value = "";
		}
	}
	//initialise these values into GameSystem
	_gameOver = false;
	_mapSizeX = mapX;
	_mapSizeY = mapY;
	_starCount = starCount;

	char block = 178;
	char breakBlock = 176;
	Tile Wall(block, true, false, 0); //wall tile passes (character displayed, whether the tile is passable)
	Tile BreakWall(breakBlock, true, false, 0);
	Tile Air(' ', false, false, 0); //air tile 
	Tile Start('0', false, false, 0); //start tile
	Tile Finish('1', false, false, 0); //finish tile
	Tile Star('*', false, false, 0); //star tile
	Tile Health('+', false, false, 0); //health tile
	Tile monsterDead('%', false, false, 0); //monster dead tile 
	//load Map

	for (int i = 0; i < _mapSizeY; i++) {
		//check characters
		char tile;
		std::string tiles;
		getline(loadMap, tiles);
		//for (int j = 0; j < tiles.size(); j++) {
		int k = 0;
		int j = 0;
		while (k < tiles.size()) {

			tile = tiles[k];
			switch (tile) { // files don't support some ascii characters so have to be represented as other characters in file
			case '#': tile = block; break; //converts a wall block
			case '/': tile = breakBlock; break; //converts a breakable block
			}
			k = k + 2; //skips the comma

			int solid = tiles[k] - '0'; //convert the character into an int
			int visible = tiles[k + 2] - '0'; //convert the character into an int

			Tile loadTile(tile, solid, visible, 0);
			_map[i][j] = loadTile; //places tile in map array
			j++;
			k = k + 4;
		}
		k = 0;
		j = 0;
	}
	//load player and monsters
	std::ifstream loadSprites("SpriteSave.txt");
	int posX, posY, level, HP, numDice, minRange, maxRange, mazeLevel, killCount, exp, movementSpeed = 0, movementCount = 0;
	char spriteChar, tile;
	bool spriteSolid, playerFound = false;
	Tile tileOn;

	count = 0;
		getline(loadSprites, gameSystemSetUp);
		for (int i = 0; i < gameSystemSetUp.size(); i++) {
			if (gameSystemSetUp[i] != ',') { //skips commas
				value = value + gameSystemSetUp[i];
			}
			else {
				count++; //increases count for the case to know which piece of data it is on
				switch (count) { //variable names describe their purpose
				std::cout << atoi(value.c_str()) << std::endl;
				case 1: posX = atoi(value.c_str()); break; 
				case 2: posY = atoi(value.c_str()); break;
				case 3: spriteChar = value[0]; break;
				case 4: spriteSolid = atoi(value.c_str()); break;
				case 5: tile = value[0];
					switch (tile) {  //may need to change this to remember which part of map has been visited or not
					case '#': tileOn = Wall; break;
					case '/': tileOn = BreakWall; break;
					case ' ': tileOn = Air; break;
					case '0': tileOn = Start; break;
					case '1': tileOn = Finish; break;
					case '*': tileOn = Star; break;
					case '+': tileOn = Health; break;
					case '%': tileOn = monsterDead; break;
					}
					break;
				case 6: level = atoi(value.c_str()); break;
				case 7: HP = atoi(value.c_str()); break;
				case 8: numDice = atoi(value.c_str()); break;
				case 9: minRange = atoi(value.c_str()); break;
				case 10: maxRange = atoi(value.c_str()); break;
				case 11: mazeLevel = atoi(value.c_str()); break;
				case 12: killCount = atoi(value.c_str()); break;
				case 13: exp = atoi(value.c_str()); break;
				case 14: starCount = atoi(value.c_str()); break;
				}
				value = "";
			}
		}

		//add this to a player array
		Player player(posX, posY, spriteChar, spriteSolid, tileOn, level, HP, numDice, minRange, maxRange, mazeLevel, killCount, exp, starCount);
		player;

		//)
		if (loadSprites.eof() != true) {
			while (loadSprites.eof() == false) { //this shouldn't loop if there are no monsters
				count = 0;
				getline(loadSprites, gameSystemSetUp); 
				if (gameSystemSetUp != "") { //loops through every monster until all have been loaded
					for (int i = 0; i < gameSystemSetUp.size(); i++) {//same method as the player for less variables 
						if (gameSystemSetUp[i] != ',') {
							value = value + gameSystemSetUp[i];
						}
						else {
							count++;
							switch (count) {
							case 1: posX = atoi(value.c_str()); break;
							case 2: posY = atoi(value.c_str()); break;
							case 3: spriteChar = value[0]; break;
							case 4: spriteSolid = atoi(value.c_str()); break;
							case 5: tile = value[0];  
								switch (tile) {
								case '#': tileOn = Wall; break;
								case '/': tileOn = BreakWall; break;
								case ' ': tileOn = Air; break;
								case '1': tileOn = Finish; break;
								case '0': tileOn = Start; break;
								case '*': tileOn = Star; break;
								case '+': tileOn = Health; break;
								case '%': tileOn = monsterDead; break;
								}
								break;
							case 6: level = atoi(value.c_str()); break;
							case 7: HP = atoi(value.c_str()); break;
							case 8: numDice = atoi(value.c_str()); break;
							case 9: minRange = atoi(value.c_str()); break;
							case 10: maxRange = atoi(value.c_str()); break;
							case 11: playerFound = atoi(value.c_str()); break;
							case 12: movementSpeed = atoi(value.c_str()); break;
							case 13: movementCount = atoi(value.c_str()); break;
							}
							value = "";
						}
					}
					//add this to a monster constructor and push onto monsters vector
					//int posX, int posY, char monsterChar, bool monsterSolid, Tile tileOn, int level, int HP, int numDice, int minRange, int maxRange, bool playerFound
					Monster monster(posX, posY, spriteChar, spriteSolid, tileOn, level, HP, numDice, minRange, maxRange, playerFound, movementSpeed, movementCount);
					Monsters.push_back(monster); //need to update to allows for movement speed/count
					drawEntities(monster); //adds monsters to the map
				}
			}
		}
		std::cout << "GAME LOADED" << std::endl;
		drawEntities(player); 
		createVisionMaze(player);
		drawMap();
		GUI(player);
		gameLoop(player);
}

void GameSystem::saveGame(Player &player)
{
	//save map information
	//Map information saved first then layout

	std::ofstream saveMap("MapSave.txt", std::ofstream::out);

	saveMap << _mapSizeX << "," << _mapSizeY << "," << _starCount << "," << std::endl;

	for (int i = 0; i < _mapSizeY; i++) { //loops through all Y values 
		for (int j = 0; j < _mapSizeX; j++) { //loops through all X values
			char tile = _map[i][j].getTileChar(); //gets the tile from the map

			switch (int(tile)) {
			case -78: tile = '#'; //need to convert ascii into character so text editor can understand
				break;
			case -80: tile = '/';
				break;
			default: //tilechar is a legal character for text editor
				break;
			}

			saveMap << tile << "," << _map[i][j].getTileSolid() << "," << _map[i][j].getVisited() << "|"; //add to file 

		}
		saveMap << std::endl;
	}
	saveMap.close();

	//save sprite information
	//player saved first then all other monster sprites

	std::ofstream spriteSave("SpriteSave.txt", std::ofstream::out);
	
	//Player
	int x, y, numDice, minRange, maxRange;
	player.getSpritePos(x, y);
	player.getDice(numDice, minRange, maxRange);
	Tile TileOn = player.getTileOn();
	//collect player information and add it to the file
	spriteSave << x << "," << y << "," << player.getSpriteChar() << "," << player.getSpriteSolid() << "," << TileOn.getTileChar() << "," << player.getLevel()
		<< "," << player.getHP() << "," << numDice << "," << minRange << "," << maxRange << "," << player.getMazeLevel() << "," << player.getKillCount()
		<< "," << player.getExp() << "," << player.getStarCount() << "," << std::endl;

	//collect monsters information and add it to the file
	//int posX, int posY, char monsterChar, bool monsterSolid, Tile tileOn, int level, int HP, int numDice, int minRange, int maxRange, bool playerFound);
	if (Monsters.size() != 0) {
		for (int i = 0; i < Monsters.size(); i++) {
			Monsters[i].getSpritePos(x, y); //gets the monsters x,y position
			Monsters[i].getDice(numDice, minRange, maxRange); //gets the monsters dice values for storing
			Tile TileOn = Monsters[i].getTileOn(); //gets the tile the monster is on
			//save data to file
			spriteSave << x << "," << y << "," << Monsters[i].getSpriteChar() << "," << Monsters[i].getSpriteSolid() << "," << TileOn.getTileChar()
				<< "," << Monsters[i].getLevel() << "," << Monsters[i].getHP() << "," << numDice << "," << minRange << "," << maxRange
				<< "," << Monsters[i].getPlayerFound() << "," << Monsters[i].getMovementSpeed() << "," << Monsters[i].getMovementCount() << "," << std::endl;
		}
		spriteSave.close();
	}
}

void GameSystem::deleteSave() //opens both save files and overwrites them
{
	std::ofstream mapSave;
	mapSave.open("MapSave.txt", std::ofstream::out | std::ofstream::trunc);
	mapSave << ".";
	mapSave.close();

	std::ofstream spriteSave;
	spriteSave.open("SpriteSave.txt", std::ofstream::out | std::ofstream::trunc);
	mapSave << ".";
	spriteSave.close();
}

void GameSystem::newGame()
{
	createMaze();
	Player player = createPlayer(); //create the player class

	drawEntities(player); //used to draw the player before gamw starts
	createVisionMaze(player); //create sthe vision for the player
	drawMap(); //draws the map to teh screen
	GUI(player); //adds interface for the user

	gameLoop(player); //main game loop
}

void GameSystem::gameLoop(Player &player)
{
	bool moved;

	while (_gameOver == false) {
		//createDistanceMap(player); //may be scewing distance map
		moved = playerMove(player); //move the player // return value is now obselete
		createDistanceMap(player);
		createVisionMaze(player);
		monsterMove(player);
		drawMap();
		GUI(player);
	}
}

Player GameSystem::createPlayer()
{
	Tile Start('0', false, false, 0); //player always starts so tile under it must be start
	Player player(1, 1, '@', true, Start, 5, 100, 1, 5, 10, 1, 0, 0, 0);
	/*Player(XPosition, YPosition, character, solid, tile prevously on, health, level, #of dice, 
	range of dice, maze level, exp, kill count) */
	
	return player;
}

void GameSystem::createMonsters(int posX, int posY)
{
	
	Tile Air(' ', false, false, 0); //air tile placed under the enemies

	//declaration of enemies
	Monster cyclops(posX, posY, 'C', true, _map[posY][posX], 4, 30, 1, 1, 8, false, 1, 0);
	Tile Cyclops(cyclops.getSpriteChar(), cyclops.getSpriteSolid(), false, 0);

	Monster dragon(posX, posY, 'D', true, _map[posY][posX], 50, 200, 5, 5, 10, false, 2, 0);
	Tile Dragon(dragon.getSpriteChar(), dragon.getSpriteSolid(), false, 0);

	Monster giant(posX, posY, 'G', true, _map[posY][posX], 20, 50, 3, 3, 10, false, 2, 0);
	Tile Giant(giant.getSpriteChar(), giant.getSpriteSolid(), false, 0);

	Monster goblin(posX, posY, 'g', true, _map[posY][posX], 3, 10, 2, 3, 3, false, 0, 0);
	Tile Goblin(goblin.getSpriteChar(), goblin.getSpriteSolid(), false, 0);

	Monster orc(posX, posY, 'O', true, _map[posY][posX], 7, 25, 3, 2, 7, false, 0, 0);
	Tile Orc(orc.getSpriteChar(), orc.getSpriteSolid(), false, 0);

	Monster pixie(posX, posY, 'P', true, _map[posY][posX], 6, 10, 3, 4, 8, false, 0, 0);
	Tile Pixie(pixie.getSpriteChar(), pixie.getSpriteSolid(), false, 0);

	Monster spider(posX, posY, 'S', true, _map[posY][posX], 9, 15, 3, 3, 8, false, 0, 0);
	Tile Spider(spider.getSpriteChar(), spider.getSpriteSolid(), false, 0);

	Monster skeleton(posX, posY, 's', true, _map[posY][posX], 2, 15, 1, 3, 6, false, 0, 0);
	Tile Skeleton(skeleton.getSpriteChar(), skeleton.getSpriteSolid(), false, 0);

	Monster troll(posX, posY, 'T', true, _map[posY][posX], 3, 25, 1, 5, 10, false, 1, 0);
	Tile Troll(troll.getSpriteChar(), troll.getSpriteSolid(), false, 0);

	Monster witch(posX, posY, 'W', true, _map[posY][posX], 15, 20, 4, 3, 6, false, 0, 0);
	Tile Witch(witch.getSpriteChar(), witch.getSpriteSolid(), false, 0);

	Monster wolf(posX, posY, 'w', true, _map[posY][posX], 3, 25, 1, 2, 2, false, 0, 0);
	Tile Wolf(wolf.getSpriteChar(), wolf.getSpriteSolid(), false, 0);

	Monster zombie(posX, posY, 'Z', true, _map[posY][posX], 1, 10, 1, 2, 3, false, 1, 0);
	Tile Zombie(zombie.getSpriteChar(), zombie.getSpriteSolid(), false, 0);

	//switch of random numbers to decide enemy 
	int chooseMonster = rand() % 23 + 1; //selects 1 of 12 monsters randomly
	switch (chooseMonster) {
	case 1: 
	case 2:
		_map[posY][posX] = Cyclops;
		Monsters.push_back(cyclops);
		break;
	case 3:
		_map[posY][posX] = Dragon;
		Monsters.push_back(dragon);
		break;
	case 4:
		_map[posY][posX] = Giant;
		Monsters.push_back(giant);
		break;
	case 5:
	case 6:
	case 7:
	case 8:
		_map[posY][posX] = Goblin;
		Monsters.push_back(goblin);
		break;
	case 9:
	case 10:
		_map[posY][posX] = Orc;
		Monsters.push_back(orc);
		break;
	case 11:
		_map[posY][posX] = Pixie;
		Monsters.push_back(pixie);
		break;
	case 12:
		_map[posY][posX] = Spider;
		Monsters.push_back(spider);
		break;
	case 13:
	case 14:
	case 15:
		_map[posY][posX] = Skeleton;
		Monsters.push_back(skeleton);
		break;
	case 16:
		_map[posY][posX] = Troll;
		Monsters.push_back(troll);
		break;
	case 17:
	case 18:
		_map[posY][posX] = Witch;
		Monsters.push_back(witch);
		break;
	case 19:
	case 20:
		_map[posY][posX] = Wolf;
		Monsters.push_back(wolf);
		break;
	case 21:
	case 22:
	case 23:
		_map[posY][posX] = Zombie;
		Monsters.push_back(zombie);
		break;
	}

}

void GameSystem::createMaze()
{
	srand(time(NULL)); //initialise random seed
	
	char block = 178;
	char breakBlock = 176;
	Tile Wall(block, true, false, 0); //wall tile passes (character displayed, whether the tile is passable)
	Tile BreakWall(breakBlock, true, false, 0);
	Tile Air(' ', false, false, 0); //air tile 
	Tile Start('0', false, false, 0); //start tile
	Tile Finish('1', false, false, 0); //finish tile
	Tile Star('*', false, false, 0);
	Tile Health('+', false, false, 0);

	for (int i = 0; i < _mapSizeY; i++) {
		for (int j = 0; j < _mapSizeX; j++) {
			_map[i][j].setVisited(false); //prints character of tile selected
		}
	}

	// initialise blank map checkerboard
	_starCount = 0;
	for (int i = 0; i < _mapSizeY; i++){
		for (int j = 0; j < _mapSizeX; j++){
			if ((i % 2) == 0) {  //every other line is filled with walls
				_map[i][j] = Wall;
			}
			else { // lines that are not walls alternate between air and wall
				if ((j % 2) != 0) {
					if ((rand() % 60 + 1) == 60) {
						if (((j >= 4) || (i >= 4)) && ((j < _mapSizeX - 3) || (i < _mapSizeY - 3))) { //used to vary where objects can spawn
							_map[i][j] = Star;
							_starCount++;
						}
					}
					else {
						if ((rand() % 60 + 1) == 60) {
							if (((j >= 4) || (i >= 4)) && ((j < _mapSizeX - 3) || (i < _mapSizeY - 3))) {
								_map[i][j] = Health;
							}
						}
						else {
							if (((j >= 4) || (i >= 4)) && ((j < _mapSizeX - 3) || (i < _mapSizeY - 3))) {
								double spawnRate = (5 / double(_mapSizeX)) * 100;
								spawnRate = floor(spawnRate);
								int spawnNum = rand() % int(spawnRate) + 1; //calculation for probability a monster will spawn
								if (spawnNum == 1) {
									createMonsters(j, i);
								}
							}
							else {
								_map[i][j] = Air;
							}
						}
					}
				}
				else {
					if ((rand() % 10 + 1) < 9) {
						_map[i][j] = Wall;
					}else {
						if ((i == 0) || (j == 0) || (i == _mapSizeY - 1) || (j == _mapSizeX - 1)) {
							_map[i][j] = Wall;
						}
						else {
							_map[i][j] = BreakWall;
						}
					}
					
				}
			}
		}
	}

	//pointers keep track of place on both visited and tile maps
	int pointerX = 1;
	int pointerY = 1;
	bool mazeComplete = false; //maze creation completed or not
	std::vector<int> notVisited;

	while (mazeComplete == false) { //find which tiles have not been visited
		if ((_map[pointerY - 2][pointerX].getVisited() == false) && (pointerY - 2 > 0)) {
			notVisited.push_back(1); //pushes direction on to stack
		}
		if ((_map[pointerY][pointerX + 2].getVisited() == false) && (pointerX + 2 < _mapSizeX)) {
			notVisited.push_back(2);
		}
		if ((_map[pointerY + 2][pointerX].getVisited() == false) && (pointerY + 2 < _mapSizeY)) {
			notVisited.push_back(3);
		}
		if ((_map[pointerY][pointerX - 2].getVisited() == false) && (pointerX - 2 > 0)) {
			notVisited.push_back(4);
		}

		if (notVisited.size() != 0) { //checks to see if there are any possible moves
			int select = rand() % notVisited.size() + 1; //selects random number between 1 and number of items in vector
			int direction = notVisited[select - 1]; //selects a random number from the vector
			switch (direction) { //moves in direcion chosen
			case 1:
				stackX.push_back(pointerX); //pushes current X on to stack
				stackY.push_back(pointerY); //pushes current y on to stack
				_map[pointerY - 1][pointerX] = Air; //destroys wall to connect tiles
				_map[pointerY - 2][pointerX].setVisited(true); //sets new node to visited
				pointerY -= 2; //increments pointer 
				notVisited.clear(); //clears vector to allow new directions
				break;

			case 2:
				stackX.push_back(pointerX);
				stackY.push_back(pointerY);
				_map[pointerY][pointerX + 1] = Air;
				_map[pointerY][pointerX + 2].setVisited(true);
				pointerX += 2;
				notVisited.clear();
				break;

			case 3:
				stackX.push_back(pointerX);
				stackY.push_back(pointerY);
				_map[pointerY + 1][pointerX] = Air;
				_map[pointerY + 2][pointerX].setVisited(true);
				pointerY += 2;
				notVisited.clear();
				break;

			case 4:
				stackX.push_back(pointerX);
				stackY.push_back(pointerY);
				_map[pointerY][pointerX - 1] = Air;
				_map[pointerY][pointerX - 2].setVisited(true);
				pointerX -= 2;
				notVisited.clear();
				break;
			}
		}
		else {
			if (stackX.size() != 0) {
				// makes pointer last location from stack
				pointerX = stackX.back();
				pointerY = stackY.back();

				stackX.pop_back(); //removes last location from stack
				stackY.pop_back();
			}
			else {
				mazeComplete = true;
			}
		}
	}

	_map[1][1] = Start;
	_map[_mapSizeY - 2][_mapSizeX - 2] = Finish; //places finish in bottom right corner may cahnge to be more random later

	for (int i = 0; i < _mapSizeY; i++) {
		for (int j = 0; j < _mapSizeX; j++) {
			_map[i][j].setVisited(false); //prints character of tile selected
		}
	}

} // end brace

void GameSystem::createVisionMaze(Player &player) //do this just before printing the map
{
	//add visions
	int x, y;

	for (int i = 1; i <= 4; i++) { //loop through the monsters vector to check if they are still visible
		switch (i) {
		case 1: 
			player.getSpritePos(x, y); //up
			do { //does not print the wall that it reaches 
				y--; //increases the search count
				_map[y][x - 1].setVisited(true);  //allows plaer to see walls either side of path
				_map[y][x].setVisited(true); //shows path infront of user
				_map[y][x + 1].setVisited(true);  //allows plaer to see walls either side of path
			} while (_map[y][x].getTileSolid() != true); //loop until reach solid wall
			break;
		case 2: 
			player.getSpritePos(x, y); //right
			do {
				x++;
				_map[y - 1][x].setVisited(true);
				_map[y][x].setVisited(true);
				_map[y + 1][x].setVisited(true);
			} while (_map[y][x].getTileSolid() != true);
			break;
		case 3:
			player.getSpritePos(x, y); //down
			do {
				y++;
				_map[y][x - 1].setVisited(true);
				_map[y][x].setVisited(true);
				_map[y][x + 1].setVisited(true);
			} while (_map[y][x].getTileSolid() != true);
			break;
		case 4:
			player.getSpritePos(x, y); //left
			do {
				x--;
				_map[y - 1][x].setVisited(true);
				_map[y][x].setVisited(true);
				_map[y + 1][x].setVisited(true);
			} while (_map[y][x].getTileSolid() != true);
			break;
		}
	}

}

void GameSystem::drawMap()
{
	//draw map
	for (int i = 0; i < _mapSizeY; i++) {
		for (int j = 0; j < _mapSizeX; j++) {
			if (_map[i][j].getVisited() == true) {
				std::cout << _map[i][j].getTileChar(); //prints character of tile selected
			}
			else {
				std::cout << '.';
			}
		} 
		std::cout << std::endl;
	}
	//commented below are used for testing features of the game
	
	/*for (int i = 0; i < _mapSizeY; i++) {
		for (int j = 0; j < _mapSizeX; j++) {
				std::cout << _map[i][j].getVisited(); //prints character of tile selected
		}
		std::cout << std::endl;
	} */
	
	/* for (int i = 0; i < _mapSizeY; i++) {
		for (int j = 0; j < _mapSizeX; j++) {
			if (_map[i][j].getDistanceFromObject() != 500) {
				std::cout << _map[i][j].getDistanceFromObject();
				//prints character of tile selected
			}
			else {
				std::cout << "#";
			}
		}
		std::cout << std::endl;
	}  */

} 

void GameSystem::GUI(Player player) 
{
	//prints the GUI under the maps to keep track of important information
	std::cout << "HP: " << player.getHP() << " Level: " << player.getLevel() << " Exp: " << player.getExp() <<
		" Stars: " << player.getStarCount() << "/" << _starCount << std::endl; 
}

void GameSystem::searchPlayer(Monster &monster, Player &player)
{
	//create vision for monster. -> reuse code to create vision for player?
	//try moving player forward in all directions until hits wall,
	//if player not see between enemey and wall then can't see player.

	int searchX = 1, searchY = 1, x ,y;
	monster.getSpritePos(x, y);

	for (int i = 1; i <= 4; i++) {
		switch (i) {
		case 1: //searching up (W)

			while (_map[y - searchY][x].getTileSolid() == false) { //checks whether the tile looking at is solid. if not then may be player.
				searchY++;
			}
			if (_map[y - searchY][x].getTileChar() == '@') { //checks to see if monster has found player
				monster.setPlayerFound(true); // tells monster it has seen player
			}
			searchY = 1; //resets value for other searches
			break;

		case 2://searching right (D)
			while (_map[y][x + searchX].getTileSolid() == false) {
				searchX++;
			}
			if (_map[y][x + searchX].getTileChar() == '@') { //checks to see if monster has found player
				monster.setPlayerFound(true); // tells monster it has seen player
			}
			searchX = 1;
			break;

		case 3://searching down (S)	
			while (_map[y + searchY][x].getTileSolid() == false) {
				searchY++;
			}
			if (_map[y + searchY][x].getTileChar() == '@') { //checks to see if monster has found player
				monster.setPlayerFound(true); // tells monster it has seen player
			}
			searchY = 1;
			break;

		case 4://searching left (A)
			
			while (_map[y][x - searchX].getTileSolid() == false) {
				searchX++;
			}
			if (_map[y][x - searchX].getTileChar() == '@') { //checks to see if monster has found player
				monster.setPlayerFound(true); // tells monster it has seen player
			}
			searchX = 1;
			break;
		}
			
	}
}

void GameSystem::createDistanceMap(Player &player) //reuse to test if player created maps valid
{
	// breadth first search
	char block = 178;
	char breakBlock = 176;
	int x = 0, y = 0;

	for (int i = 0; i < _mapSizeY; i++) {
		for (int j = 0; j < _mapSizeX; j++) {
			_map[i][j].setDistanceFromObject(500); //sets all tiles to a distance of 500 from player
		}
	}

	player.getSpritePos(x, y);
	Distance distanceObj;
	distanceObj.pushItems(x, y, 0);
	DistanceQueueList.push_back(distanceObj);
	std::vector<Distance> DistanceQueueTemp; //keeps hold of temp distances */

	int distance = 1; //keeps track of how far away the tile is from the player.
	while (distance < 20){
		for (int i = 1; i <= 4; i++) { //selects which direction to search in
			switch (i) { //swicth used to xelect position - used switch to make code look neater
			case 1:
				//checks in the searched tile is a wall if it is the player or if the searched tile is greater than the current tile
				if ((_map[y - 1][x].getTileChar() != block) && (_map[y - 1][x].getTileChar() != breakBlock) && (_map[y - 1][x].getDistanceFromObject() > _map[y][x].getDistanceFromObject())) { //generating distance up
					distanceObj.pushItems(x, y - 1, distance); //puts tile x, y position and direction into obj to be stored in queue
					DistanceQueueTemp.push_back(distanceObj);//puts location and distance of tile away from player into queue
					//DistanceQueueList.push_back(distanceObj);
				}
				break;
			case 2:                                             
				if ((_map[y][x + 1].getTileChar() != block) && (_map[y][x + 1].getTileChar() != breakBlock) && (_map[y][x + 1].getDistanceFromObject() > _map[y][x].getDistanceFromObject())) { //generating distance right
					distanceObj.pushItems(x + 1, y, distance);
					DistanceQueueTemp.push_back(distanceObj);
				}
				break;
			case 3:
				if ((_map[y + 1][x].getTileChar() != block) && (_map[y + 1][x].getTileChar() != breakBlock) && (_map[y + 1][x].getDistanceFromObject() > _map[y][x].getDistanceFromObject())) { //generating distance down
					distanceObj.pushItems(x, y + 1, distance);
					DistanceQueueTemp.push_back(distanceObj);
				}
				break;
			case 4:
				if ((_map[y][x - 1].getTileChar() != block) && (_map[y][x - 1].getTileChar() != breakBlock) && (_map[y][x - 1].getDistanceFromObject() > _map[y][x].getDistanceFromObject())) { //generating distance left
					distanceObj.pushItems(x - 1, y, distance);
					DistanceQueueTemp.push_back(distanceObj);
				}
				break; //sorry this searches in the complete opposite way to the movement.
			}//end of case

		} //end of for
		int x1, y1, distance1;
		int x2, y2, distance2;
		Distance distanceObj1;
		Distance distanceObj2;

		//compares all objs on the temp queue and list queue and removes duplicates from the temp queue
		for (int i = 0; i < DistanceQueueTemp.size(); i++) {
			distanceObj1 = DistanceQueueTemp[i];
			distanceObj1.popItems(x1, y1, distance1);
			for (int j = 0; j < DistanceQueueList.size(); j++) {
				distanceObj2 = DistanceQueueList[j];
				distanceObj2.popItems(x2, y2, distance2);

				if ((x1 == x2) && (y1 == y2) && (distance1 == distance2)) { // if the searched tile has the same x,y coords but less distance remove the worse distance
					DistanceQueueTemp.erase(DistanceQueueTemp.begin() + i); //removes object from temp queue
				}

			}
		} 

		for (int i = 0; i < DistanceQueueTemp.size(); i++) { //need to remove already checked tiles from the queue or never add them
			distanceObj = DistanceQueueTemp[i];
			DistanceQueueList.push_back(distanceObj);
		}
		DistanceQueueTemp.clear(); //clears the temp distance queue for the next loop

		distanceObj = DistanceQueueList.front(); //pops teh first object off the queue as assigns it to a variable
		DistanceQueueList.erase(DistanceQueueList.begin()); //deletes the object from the queue

		distanceObj.popItems(x, y, distance); //gets teh values of the object

		_map[y][x].setDistanceFromObject(distance); //sets the x,y coordinates of the popped object to the distance
		distance++; //increases the distance form teh player
	
	} //end of while 
	DistanceQueueList.clear(); //clears the main queue for next turn

}

int GameSystem::followPlayer(Monster &monster)
{
	//int distance = 1000;
	int x = 0, y = 0, move = 0;
	monster.getSpritePos(x, y);

	for (int i = 1; i <= 4; i++) {
		switch (i) {
		case 1: 
			if (_map[y - 1][x].getDistanceFromObject() < _map[y][x].getDistanceFromObject()) {
				//distance = _map[y - 1][x].getDistanceFromObject();
				move = 1;
			}
		case 2:
			if (_map[y][x + 1].getDistanceFromObject() < _map[y][x].getDistanceFromObject()) {
				//distance = _map[y][x + 1].getDistanceFromObject();
				move = 4;//switched round because search is clock wise movement is anti clockwise
			}
		case 3:
			if (_map[y + 1][x].getDistanceFromObject() < _map[y][x].getDistanceFromObject()) {
				//distance = _map[y + 1][x].getDistanceFromObject();
				move = 3;
			}
		case 4:
			if (_map[y][x - 1].getDistanceFromObject() < _map[y][x].getDistanceFromObject()) {
				//distance = _map[y][x - 1].getDistanceFromObject();
				move = 2; //switched round because search is clock wise movement is anti clockwise
			}
		} 
	}//end for
	return move;
}

void GameSystem::breakBlock(Player &player)
{
	char breakWall = 176;
	Tile Air(' ', false, false, 0); //air tile 
	int x, y;

	player.getSpritePos(x, y);
	switch (player.getPlayerDirection()) {
	case 1: if (_map[y - 1][x].getTileChar() == breakWall) { //checks to see if it is a breakable wall
				_map[y - 1][x] = Air; //replace sthe block with and unsolid air block
			}
		break;
	case 2:if (_map[y][x - 1].getTileChar() == breakWall) {
				_map[y][x - 1] = Air;
			}
		break;
	case 3: if (_map[y + 1][x].getTileChar() == breakWall) {
				_map[y + 1][x] = Air;
			}
		break;
	case 4: if (_map[y][x + 1].getTileChar() == breakWall) {
				_map[y][x + 1] = Air;
			}
		break;
	}
}

bool GameSystem::playerMove(Player &player)
{
	int x = 0, y = 0;
	bool moved = false;

	player.getSpritePos(x, y);
	char keyPressed;

	//std::cin >> keyPressed; //takes user input
	keyPressed = _getch();
	if ((keyPressed == 'w') || (keyPressed == 'a') || (keyPressed == 's') || (keyPressed == 'd') || (keyPressed == 'W') || (keyPressed == 'A') || (keyPressed == 'S') || (keyPressed == 'D') ||
		(keyPressed == 'p') || (keyPressed == 'P') || (keyPressed == 'm') || (keyPressed == 'M') || (keyPressed == 'n') || (keyPressed == 'N')) {
		switch (keyPressed) {
		case 'w':
		case 'W':moved = checkMove(x, y - 1, player); //up
			player.setPlayerDriection(1);
			break;
		case 'a':
		case 'A':moved = checkMove(x - 1, y, player); //left
			player.setPlayerDriection(2);
			break;
		case 's':
		case 'S':moved = checkMove(x, y + 1, player); //down
			player.setPlayerDriection(3);
			break;
		case 'd':
		case 'D':moved = checkMove(x + 1, y, player); //right
			player.setPlayerDriection(4);
			break;
		case 'p':
		case 'P':breakBlock(player);
			moved = false;
			break;
		case 'm':
		case 'M':saveGame(player);
			moved = false;
			std::cout << "GAME SAVED" << std::endl;
			break;
		case 'n':
		case 'N': 
			std::cout << "Are you sure you wish to quit? any unsaved data will be lost. [1] Yes [2] No" << std::endl;
			int option = 'a';
			while ((option != '1') && (option != '2')) { //error handler
				option = _getch();
				if (option == '1') {
					_gameOver = true;
					moved = false;
				}
			}
			
			break;
		}
		return moved;
	}
	else {
		//invalid input
	}
}

void GameSystem::monsterMove(Player &player)
{
	int move = 1;

	for (int i = 0; i < Monsters.size(); i++) { //loops through all monsters to make their moves
		searchPlayer(Monsters[i], player); //cuntion that searches for the player 

		Tile TileOn = Monsters[i].getTileOn();
		if ((TileOn.getDistanceFromObject() >= 20) && (Monsters[i].getPlayerFound() == true)) { //checks to see if player is in range
			Monsters[i].setPlayerFound(false); //if the monster is 20 tiles from the player then reset playerfound to false
		}

		if (Monsters[i].getPlayerFound() == true) { 
			//Search for player every turn to check if still visible.
			// follow the player - Monsters[i].follow
			if (Monsters[i].getMovementSpeed() == Monsters[i].getMovementCount()) {
				move = followPlayer(Monsters[i]); //get the movemenrt of the monster
				Monsters[i].setMovementCount(0); //reset the monster movement count
			}
			else {
				Monsters[i].setMovementCount(Monsters[i].getMovementCount() + 1); //if they don't move then increase a monster move count
				move = 5; //idor move
			}
		}
		else {
			move = rand() % 5 + 1; //random move for monster
		}

			int x, y;
			Monsters[i].getSpritePos(x, y); //get the monsters x and y position

			switch (move) {
			case 1:checkMove(x, y - 1, Monsters[i]); //up
				break;
			case 2:checkMove(x - 1, y, Monsters[i]);//left
				break;
			case 3:checkMove(x, y + 1, Monsters[i]); //down
				break;
			case 4:checkMove(x + 1, y, Monsters[i]); //right
				break;
			case 5: //idol
				break;
			}
		
	}
}

bool GameSystem::checkMove(int x, int y, Sprite &sprite) //change this to allow for any sprite
{//doesn't allow monsters to engage in comabt with player
	int currentX, currentY, x1, y1;
	sprite.getSpritePos(currentX, currentY); //gets current player x and y position

	if (sprite.getSpriteChar() == '@') { //remove to add combat with monsters vs monsters
		if (((_map[y][x].getTileChar() >= 63) && (_map[y][x].getTileChar() <= 90)) || ((_map[y][x].getTileChar() >= 97) && (_map[y][x].getTileChar() <= 122))) {
			//very inefficient way - should have found way to pass monster in as pointer
			for (int i = 0; i < Monsters.size(); i++) { //searches for the monster that the player collided with
				Monsters[i].getSpritePos(x1, y1); //gets the x and y coordinates of the monsters
				if ((x1 == x) && (y1 == y)) {
					Monster monster = Monsters[i];
					combat(sprite, monster, i); //maybe add feature where monsters can battle other monsters to gain levels?
				}
			}

		}
	}

	Tile Air(' ', false, false, 0); //picking up stars
	if ((_map[y][x].getTileChar() == '*') && (_map[currentY][currentX].getTileChar() == '@')) {
		sprite.setStarCount(sprite.getStarCount() + 1); //increase star count of player
		_map[y][x] = Air; //set the star to an air tile
	}

	if ((_map[y][x].getTileChar() == '+') && (_map[currentY][currentX].getTileChar() == '@')) {
		sprite.setHP(sprite.getHP() + 25); //increase the players health by 25
		_map[y][x] = Air; //replace the health with an air tile
		std::cout << "Health increased" << std::endl;
	}


	if ((_map[y][x].getTileChar() == '1') && (_map[currentY][currentX].getTileChar() == '@') && (sprite.getStarCount() == _starCount)){
		if (_mapSizeX < 71) { //used to make sure map does not increase past console size
			_mapSizeX += 4; //increase map X size by 4
			_mapSizeY += 2; //increase map Y size by 2
		}
		
		_starCount = 0; //reset star count of the map

		Tile Start('0', false, true, 0); //use for moving the player to the start of the map

		sprite.setStarCount(0); //reset players star count
		Monsters.clear(); //remove any remaing monsters form teh level

		Tile reset(' ', false, false, 0);
		for (int i = 0; i < _mapSizeY; i++) {
			for (int j = 0; j < _mapSizeX; j++) {
				_map[i][j] = reset; //reset all tiles
			}
		}

		createMaze(); //create a new maze

		sprite.setTileOn(Start);//cahnge so '0' appears under player after moved.
		sprite.setSpritePos(1, 1); //moves player back to start of map
		drawEntities(sprite);
		drawMap();

		return true;
	}
	else {
		if ((_map[y][x].getTileChar() == '1') && (_map[currentY][currentX].getTileChar() == '@')) {
			//if the player has not collect all the stars in the map then
			std::cout<< "Stars left to collect: " << _starCount - sprite.getStarCount() << std::endl;
		}
	}

	if ((_map[y][x].getTileSolid() == false) && (_map[y][x].getVisited() == true)) { // if the tile is solid allow player to move
		_map[currentY][currentX] = sprite.getTileOn(); //makes the current map position = to what the entity was on
		sprite.setTileOn(_map[y][x]); //sets tileOn to new position
		sprite.setSpritePos(x, y); // sets new player position
		drawEntities(sprite);
		return true; // returns that the player physically moved
	}
	else {
		return false;
	}
	return false;
}

void GameSystem::drawEntities(Sprite &sprite) //need way to know if monsters are visible
{
	int x, y;
	
	sprite.getSpritePos(x, y); //gets sprites x and y position
	Tile spriteTile = sprite.getTileOn();
	if ((spriteTile.getVisited() == true) || (sprite.getSpriteChar() == '@')) {
		Tile Sprite(sprite.getSpriteChar(), sprite.getSpriteSolid(), true, spriteTile.getDistanceFromObject()); //creates a tile from sprite values
		_map[y][x] = Sprite; //places tile into map
	}
	else {
		Tile Sprite(sprite.getSpriteChar(), sprite.getSpriteSolid(), false, spriteTile.getDistanceFromObject()); //creates a tile from sprite values
		_map[y][x] = Sprite; //places tile into map
	}
	//possibly better solution? maybe don't have to reprint entire map? change only specific values on screen
}

void GameSystem::combat(Sprite &player, Monster &monster, int monsterNum)
{
	//roll a dice to see who goes first
	int moveFirst = rand() % 2 + 1; //randomly select who goes first, monster or player
	bool winCombat = false; //holds whether player won or lost combat
	bool combatEnd = false; //hold whether combat has ended or not

	while (combatEnd == false) {
		int playerDamage = 0;
		int monsterDamage = 0;
		int playerHP = 0;
		int monsterHP = 0;

		player.rollDice(playerDamage); //get the damage of teh player
		monster.rollDice(monsterDamage); //get the damage of the monster

		playerHP = player.getHP(); //get the players HP
		monsterHP = monster.getHP(); //get the monsters HP
		int damageResult;

		for (int i = 0; i < 2; i++) {
			switch (moveFirst) {
			case 1: damageResult = monsterHP - playerDamage; //calculates the damage result
				if (damageResult > 0) { //if the monster is not dead
					monster.setHP(damageResult); //set it's HP to the new value
					std::cout << "Player did " << playerDamage << "damage - Monster health: " << damageResult << std::endl;
					moveFirst++;
				}
				else {
					//enemy dead
					std::cout << "Player did " << playerDamage <<  "damage - Monster health: 0" << std::endl;
					winCombat = true; //player has won combat
					combatEnd = true;
				}
				break;

			case 2:damageResult = playerHP - monsterDamage; //calculate the players resulting health
				if (damageResult > 0) {
					player.setHP(damageResult);
					std::cout << "Monster did " << monsterDamage << "damage - Player health: " << damageResult << std::endl;
					moveFirst--;
				}
				else {
					//player dead
					std::cout << "Monster did " << monsterDamage << "damage - Player health: 0" << std::endl;
					winCombat = false;
					combatEnd = true;
				}
				break;
			}
		}
	}

	if (winCombat) {
		int x, y;
		Tile tileOn = monster.getTileOn();
		monster.getSpritePos(x, y);

		if (_map[y][x].getTileChar() == ' ') { //changed to two if's because if/else didn't work for some reason.
			Tile monsterDead1('%', false, false, tileOn.getDistanceFromObject()); //when and enemy dies leaves %
			_map[y][x] = monsterDead1;
		}

		if ((_map[y][x].getTileChar() == '*') || (_map[y][x].getTileChar() == '0') || (_map[y][x].getTileChar() == '1') || (_map[y][x].getTileChar() != '+')) { //use to make sure that a dead monster doesn't delete important tiles
			//makes sure if the monsters dies on an important tile e.g. finish, health, star then it remains
			Tile monsterDead2(tileOn.getTileChar(), false, false, tileOn.getDistanceFromObject());
			_map[y][x] = monsterDead2;
		}

		int playerExp = player.getExp() + (monster.getLevel() * 10); //exp calculated by monsers level multiplied by 10
		player.setExp(playerExp); //setting the players new exp
		
		while (playerExp >= (player.getLevel() * 100)) { //calculating if player has leveled up
			playerExp = playerExp - (player.getLevel() * 100); //calcuates the remaining exp and add it to new count
			player.setExp(playerExp); //sets new exp of player
			player.setLevel(player.getLevel() + 1); //increases level of the player

			player.setHP(player.getHP() + 100); //increase the plaers health by 100 for leveling up
			
			int numDice, minRange, maxRange;
			player.getDice(numDice, minRange, maxRange);
			player.setDice(numDice + 1, minRange, maxRange); //increases the number of dice the player has by 1
		}

		Monsters.erase(Monsters.begin() + monsterNum); //remove the monster from the level
	}
	else {
		std::cout <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"  ___                                                                         " << std::endl <<
		" -   ---___- ,,                                ,       |_                |_   " << std::endl <<
		"    (' ||    ||                    _          ||       ||          _     ||   " << std::endl <<
		"   ((  ||    ||/||  /'|| || ||    < |, ,._-_ =||=     / ||  _-_   < |,  / ||  " << std::endl <<
		"  ((   ||    || || || || || ||    /-||  ||    ||     || || || ||  /-|| || ||  " << std::endl <<
		"   (( //     || || || || || ||   ((_||  ||    ||     || || || /  ((_|| || ||  " << std::endl <<
		"     -____-  || |/ ||_/  ||//       ||  ||,   ||,    ||_/  ||,/     || ||_/   " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl <<
		"                                                                              " << std::endl;
		deleteSave(); //delete the current save
		system("PAUSE");
		_gameOver = true; //game is over
	}
}