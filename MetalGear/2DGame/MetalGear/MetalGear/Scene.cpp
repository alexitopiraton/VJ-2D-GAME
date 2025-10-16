#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <fstream>
#include <sstream>


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 15
#define INIT_PLAYER_Y_TILES 16

#define NUM_LEVELS 13


Scene::Scene()
{
	player = NULL;
	activeLevel = NULL;
}

Scene::~Scene()
{
	if(player != NULL)
		delete player;
	if (activeLevel != NULL)
		delete activeLevel;
}

/* INIT
* Initialise shaders
* Create the player and calls its initialisation
* Initialises all playable levels and 
* Assigns the actual TileMap to the Player
* Initialises camera type (orthogonal) and currentTime
* Initialises level change delay variables
*/
void Scene::init()
{
	initShaders();

	player = new Player();
	player->init(texProgram);

	initialise_levels();

	TileMap* map = activeLevel->get_tile_map();
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;

	changingLevel = false;
	levelChangeDelay = 0.f;
	pauseGame = false;
}

void Scene::update(int deltaTime)
{
	gameTime += deltaTime;

	if (changingLevel)
	{
		levelChangeDelay += deltaTime;
		if (levelChangeDelay >= LEVEL_CHANGE_COOLDOWN)
		{
			changingLevel = false;
			levelChangeDelay = 0.f;
			stop_pause();
			cout << "Cooldown terminado" << endl;
		}
	}

	if (!pauseGame)
	{
		currentTime += gameTime;

		player->update(deltaTime);

		int tileType;
		char direction;
		if (player->changeMap_tile(tileType, direction) && !changingLevel)
		{

			int previousLevel = levelNum;

			if (tileType == 2)
				levelNum++;

			else if (tileType == 3)
				levelNum--;

			else if (tileType == 4)
				levelNum += 2;

			else if (tileType == 5)
				levelNum -= 2;


			activeLevel = levels[levelNum];
			TileMap* map = activeLevel->get_tile_map();
			player->setTileMap(map);

			glm::ivec2 playerPos = player->getPosition();
			int posX, posY;

			// Types of transitions
			// level03 -> level04 PATH
			if (levelNum == 4 && previousLevel == 3)
			{
				posX = playerPos.x - map->getTileSize();
				posY = playerPos.y - 14 * map->getTileSize();

				player->setPosition(glm::vec2(posX, posY));
				player->lookLeft();
				cout << "level03 -> level04 PATH" << endl;
			}
			// level04 -> level03 PATH
			else if (levelNum == 3 && previousLevel == 4)
			{
				posX = playerPos.x - map->getTileSize();
				posY = playerPos.y + 14 * map->getTileSize();

				player->setPosition(glm::vec2(posX,posY));
				player->lookLeft();
				cout << "level04 -> level03 PATH" << endl;
			}
			// level04 -> level05 DOOR
			else if (levelNum == 5 && previousLevel == 4)
			{
				posX = playerPos.x;
				posY = 18 * map->getTileSize();

				player->setPosition(glm::vec2(posX, posY));
				player->lookUp();
				cout << "level04 -> level05 DOOR" << endl;
			}
			// level05 -> level04 DOOR
			else if (levelNum == 4 && previousLevel == 5)
			{
				player->setPosition(glm::vec2(playerPos.x, 4 * map->getTileSize()));
				player->lookDown();
				cout << "level05 -> level04 DOOR" << endl;
			}
			// level06 -> level07 DOOR
			else if (levelNum == 7 && previousLevel == 6)
			{
				posX = playerPos.x - 13 * map->getTileSize();
				posY = 19 * map->getTileSize();

				player->setPosition(glm::vec2(posX, posY));
				player->lookUp();
				cout << "level05 -> level06 DOOR" << endl;
			}
			// level07 -> level06 DOOR
			else if (levelNum == 6 && previousLevel == 7)
			{
				posX = playerPos.x + 13 * map->getTileSize();
				posY = 7 * map->getTileSize();

				player->setPosition(glm::vec2(posX, posY));
				player->lookDown();
				cout << "level06 -> level05 DOOR" << endl;
			}
			// level11 -> level12 DOOR
			else if (levelNum == 12 && previousLevel == 11)
			{
				posX = 2 * map->getTileSize();
				posY = playerPos.y - 4 * map->getTileSize();

				player->setPosition(glm::vec2(posX, posY));
				player->lookRight();
				cout << "level11 -> level12 DOOR" << endl;
			}
			// level12 -> level11 DOOR
			else if (levelNum == 11 && previousLevel == 12)
			{
				posX = (map->getMapSize().x - 4) * map->getTileSize();
				posY = playerPos.y + 4 * map->getTileSize();

				player->setPosition(glm::vec2(posX, posY));
				player->lookLeft();
			}
			// LEFT
			else if (direction == 'L')
			{
				player->setPosition(glm::vec2((map->getMapSize().x - 4) * map->getTileSize(), playerPos.y));
				player->lookLeft();
				cout << "LEFT" << endl;
			}
			// RIGHT
			else if (direction == 'R')
			{
				player->setPosition(glm::vec2(map->getTileSize(), playerPos.y));
				player->lookRight();
				cout << "RIGHT" << endl;
			}
			// UP
			else if (direction == 'U')
			{
				posY = 19 * map->getTileSize();

				player->setPosition(glm::vec2(playerPos.x, posY));
				player->lookUp();
				cout << "UP" << endl;
			}
			// DOWN
			else if (direction == 'D')
			{
				player->setPosition(glm::vec2(playerPos.x, map->getTileSize()));
				player->lookDown();
				cout << "DOWN" << endl;
			}

			levelChangeDelay = 0.f;
			changingLevel = true;
			pause();
			cout << "TILE TYPE " << tileType << " Direction: " << direction << endl;
			cout << "Cooldown activado - cambiando a nivel " << levelNum << endl;
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	activeLevel->render();
	player->render();
	/*if (pauseGame)
		activeLevel->setBlackScreen();*/
}

/* INITIALISE_LEVELS INFO
* creates and initialises each level. Stores them in levels vector
* asigns 0 to activeLevel, position 0 of levels vector corresponding to the first level of the game
*/
void Scene::initialise_levels()
{
	ifstream fin;
	string line, tilesheetFile;
	string screensPositionFile = "images/screensPositionInSpritesheet.txt";
	string screenLevel;
	
	glm::vec2 screensPosition;

	fin.open(screensPositionFile.c_str());
	if (!fin.is_open())
		return;
	getline(fin, line);
	if (line.compare(0, 21, "SPRITESHEET POSITION") != 0)
		return;

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		Level* level = new Level();

		getline(fin, line);
		stringstream ss1(line);
		ss1 >> screenLevel;

		getline(fin, line);
		stringstream ss2(line);
		ss2 >> screensPosition[0] >> screensPosition[1];

		if (i < 5)
			level->init(screenLevel, "images/Outside_Screens/outsideScreens.png", glm::vec2(0, 0), texProgram, true, screensPosition);
		else
			level->init(screenLevel, "images/Indoor_Screens/indoorScreens.png", glm::vec2(0, 0), texProgram, false, screensPosition);

		levels.push_back(level);
	}

	fin.close();
	levelNum = 11;
	activeLevel = levels[levelNum];
}

void Scene::pause()
{
	pauseGame = true;
	player->setPause();
	activeLevel->setPause();
	cout << "PAUSE" << endl;
}

void Scene::stop_pause()
{
	pauseGame = false;
	player->setStopPause();
	activeLevel->setStopPause();
	glm::ivec2 playerPos = player->getPosition();
	cout << "PLAYER POS X Y --> " << playerPos.x / 20 << " " << playerPos.y / 20 << endl;
	cout << "CONTINUE" << endl;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



