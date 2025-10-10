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
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	

	if (changingLevel)
	{
		levelChangeDelay += deltaTime;
		if (levelChangeDelay >= LEVEL_CHANGE_COOLDOWN)
		{
			changingLevel = false;
			levelChangeDelay = 0.f;
			cout << "Cooldown terminado" << endl;
		}
		else
		{
			player->update(deltaTime);
			return;
		}

	}
	
	player->update(deltaTime);

	int tileType;
	char direction;
	if (player->changeMap_tile(tileType, direction))
	{
		cout << "TILE TYPE " << tileType << " Direction: " << direction << endl;
		cout << "levelNum " << levelNum << endl;

		int levelNumCopy = levelNum;

		if (tileType == 2)
			levelNum++;

		else if (tileType == 3)
			levelNum--;

		else if (tileType == 4)
			levelNum += 2;

		else if (tileType == 5)
			levelNum -= 2;

		if (levelNum < 0 || levelNum >= levels.size())
		{
			cout << "ERROR: level out of range" << endl;
			levelNum = levelNumCopy;
			return;
		}

		cout << "levelNum después: " << levelNum << endl;

		
		activeLevel = levels[levelNum];
		TileMap* map = activeLevel->get_tile_map();
		player->setTileMap(map);
		
		glm::ivec2 playerPos = player->getPosition();

		// LEFT
		if (direction == 'L')
			player->setPosition(glm::vec2((map->getMapSize().x - 2) * map->getTileSize(), playerPos.y));
		// RIGHT
		else if (direction == 'R')
			player->setPosition(glm::vec2(map->getTileSize(), playerPos.y));
		// UP
		else if (direction == 'U')
			player->setPosition(glm::vec2(playerPos.x, (map->getMapSize().y - 2) * map->getTileSize()));
		// DOWN
		else if (direction == 'D')
			player->setPosition(glm::vec2(playerPos.x, map->getTileSize()));

		


		levelChangeDelay += deltaTime;
		changingLevel = true;
		cout << "Cooldown activado - cambiado a nivel " << levelNum << endl;
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

	for (int i = 0; i < NUM_LEVELS-1; i++)
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
	levelNum = 0;
	activeLevel = levels[levelNum];
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



