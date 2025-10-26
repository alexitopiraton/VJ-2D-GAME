#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <fstream>
#include <sstream>


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

	characters = new Characters();
	characters->init(texProgram);
	characters->addText("transition", "LOADING...", glm::vec2(20, 440));
	characters->hideText("transition");

	TileMap* map = activeLevel->get_tile_map();
	doorOpen = map->isDoorOpen();

	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->setLevel(activeLevel);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	glViewport(0, GUI_HEIGHT, GAME_WIDTH, GAME_HEIGHT);
	currentTime = 0.0f;

	changingLevel = false;
	levelChangeDelay = 0.f;
	pauseGame = false;

	initGameOver();
}

void Scene::update(int deltaTime)
{
	gameTime += deltaTime;

	if (player->isDead())
	{
		static float deathTimer = 0.f;
		deathTimer += deltaTime;

		if (deathTimer > 3000.f) { // 3 segundos
			std::cout << "[Scene] Game Over - Volviendo al menú" << std::endl;

			// Resetear timer
			deathTimer = 0.f;

			// Llamar al reset del juego desde Game
			Game::instance().returnToMenuFromGame();
		}
		return;
	}

	if (changingLevel)
	{
		levelChangeDelay += deltaTime;
		if (levelChangeDelay >= LEVEL_CHANGE_COOLDOWN)
		{
			changingLevel = false;
			levelChangeDelay = 0.f;
			stop_pause();
		}
	}

	if (!pauseGame)
	{
		currentTime += gameTime;

		player->update(deltaTime);
		activeLevel->update(deltaTime, player);

		if (player->getCollectAllItems())
		{
			collectAllObjects();
			player->resetCollectAllItems();
		}

		if (player->getChangeMap())
		{
			levelNum = player->getMapToChange();
			activeLevel = levels[levelNum];
			TileMap* map = activeLevel->get_tile_map();
			player->setTileMap(map);
			player->setLevel(activeLevel);

			if (levelNum == 5)
			{
				player->setPosition(glm::vec2(19 * map->getTileSize(), 18 * map->getTileSize()));
				player->lookUp();
			}
			else if (levelNum == 12)
			{
				player->setPosition(glm::vec2(2 * map->getTileSize(), 12 * map->getTileSize()));
				player->lookRight();
			}

			player->setChangeMap();
		}

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

			TileMap* currentMap = activeLevel->get_tile_map();
			doorOpen = currentMap->isDoorOpen();

			glm::ivec2 playerPos = player->getPosition();
			int posX, posY;
			activeLevel = levels[levelNum];
			TileMap* map = activeLevel->get_tile_map();
			

			// Types of transitions
			// level04 -> level05 DOOR
			if (levelNum == 5 && previousLevel == 4)
			{
				if (doorOpen)
				{
					posX = playerPos.x;
					posY = 18 * map->getTileSize();

					player->setPosition(glm::vec2(posX, posY));
					player->lookUp();
					cout << "level04 -> level05 DOOR ABIERTA - CAMBIANDO" << endl;

					levels[4]->setDoorOpen(false);
				}
				else
				{
					levelNum = previousLevel;
					activeLevel = levels[levelNum];
					map = activeLevel->get_tile_map();
					cout << "level04 -> level05 DOOR CERRADA - NO CAMBIO" << endl;
					return;
				}
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
				if (doorOpen)
				{
					posX = playerPos.x - 13 * map->getTileSize();
					posY = 19 * map->getTileSize();

					player->setPosition(glm::vec2(posX, posY));
					player->lookUp();
					cout << "level06 -> level07 DOOR ABIERTA - CAMBIANDO" << endl;

					levels[6]->setDoorOpen(false);
				}
				else
				{
					levelNum = previousLevel;
					activeLevel = levels[levelNum];
					map = activeLevel->get_tile_map();
					cout << "level06 -> level07 DOOR CERRADA - NO CAMBIO" << endl;
					return;
				}

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
				if (doorOpen)
				{
					posX = 2 * map->getTileSize();
					posY = playerPos.y - 4 * map->getTileSize();

					player->setPosition(glm::vec2(posX, posY));
					player->lookRight();
					cout << "level11 -> level12 DOOR ABIERTA - CAMBIANDO" << endl;

					levels[11]->setDoorOpen(false);
				}
				else
				{
					levelNum = previousLevel;
					activeLevel = levels[levelNum];
					map = activeLevel->get_tile_map();
					cout << "level11 -> level12 DOOR CERRADA - NO CAMBIO" << endl;
					return;
				}

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

			activeLevel = levels[levelNum];
			map = activeLevel->get_tile_map();
			player->setTileMap(map);
			player->setLevel(activeLevel);

			levelChangeDelay = 0.f;
			changingLevel = true;

			pause();
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
	if (pauseGame)
		activeLevel->setBlackScreen();

	characters->render();

	if (player->isDead()) {
		renderGameOver();
	}
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
	
	int numObjects;
	int iterator = 0;
	glm::vec2 screensPosition;
	std::vector<int> accessCardNumbers;

	fin.open(screensPositionFile.c_str());
	if (!fin.is_open())
		return;
	getline(fin, line);
	if (line.compare(0, 21, "SPRITESHEET POSITION") != 0)
		return;

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		std::vector<string> objectTypes;
		std::vector<std::pair<int, int>> objectPositions;

		getline(fin, line);
		stringstream ss(line);
		ss >> numObjects;
		cout << numObjects << endl;

		// READING OBJECTS
		for (int j = 0; j < numObjects; j++)
		{
			string type; 
			getline(fin, line);
			stringstream ss00(line);
			ss00 >> type;

			if (type == "ACCESS_CARD")
			{
				int num;
				getline(fin, line);
				stringstream ss001(line);
				ss001 >> num;
				accessCardNumbers.push_back(num);
				cout << num << endl;
			}

			std::pair<int, int> position;
			getline(fin, line);
			stringstream ss01(line);
			ss01 >> position.first >> position.second;
			cout << position.first << " " << position.second << endl;

			objectTypes.push_back(type);
			objectPositions.push_back(position);
		}

		// READING GUARDS
		int guardsNum;
		std::vector<glm::vec2> guardsPositions;
		getline(fin, line);
		stringstream ss1(line);
		ss1 >> guardsNum;
		cout << guardsNum << endl;

		for (int j = 0; j < guardsNum; j++)
		{
			glm::vec2 position;
			getline(fin, line);
			stringstream ss10(line);
			ss10 >> position.x >> position.y;
			cout << position.x << " " << position.y << endl;
			guardsPositions.push_back(position);
		}


		// READING ROLLERS
		int rollersNum;
		std::vector<glm::vec2> rollersPositions;
		getline(fin, line);
		stringstream ss2(line);
		ss2 >> rollersNum;
		cout << rollersNum << endl;

		for (int j = 0; j < rollersNum; j++)
		{
			glm::vec2 position;
			getline(fin, line);
			stringstream ss20(line);
			ss20 >> position.x >> position.y;
			cout << position.x << " " << position.y << endl;
			rollersPositions.push_back(position);
		}

		// READING BOSSES
		bool arnoldBoss, twin;
		glm::vec2 arnoldPos;
		glm::vec2 twinPos;
		getline(fin, line);
		stringstream ss3(line);
		ss3 >> arnoldBoss;
		cout << arnoldBoss << endl;

		if (arnoldBoss)
		{
			getline(fin, line);
			stringstream ss30(line);
			ss30 >> arnoldPos.x >> arnoldPos.y;
		}

		getline(fin, line);
		stringstream ss4(line);
		ss4 >> twin;
		cout << twin << endl;

		if (twin)
		{
			getline(fin, line);
			stringstream ss40(line);
			ss40 >> twinPos.x >> twinPos.y;
		}

		// INITIALISING LEVELS
		Level* level = new Level();

		getline(fin, line);
		stringstream ss5(line);
		ss5 >> screenLevel;
		cout << screenLevel << endl;

		getline(fin, line);
		stringstream ss6(line);
		ss6 >> screensPosition[0] >> screensPosition[1];
		cout << screensPosition[0] << " " << screensPosition[1] << endl;

		if (i < 5)
		{
			level->init(screenLevel, "images/Outside_Screens/outsideScreens.png", glm::vec2(0, 0), texProgram, true, screensPosition, objectTypes, objectPositions, i, accessCardNumbers, iterator);
			
			if (i == 4)
				level->setDoorOpen(false);
			else
				level->setDoorOpen(true);
		}
		else
		{
			level->init(screenLevel, "images/Indoor_Screens/indoorScreens.png", glm::vec2(0, 0), texProgram, false, screensPosition, objectTypes, objectPositions, i, accessCardNumbers, iterator);
		
			if(i == 6 || i == 11)
				level->setDoorOpen(false);
			else
				level->setDoorOpen(true);
		}

		levels.push_back(level);

		// ADDING ENEMIES
		// GUARDS
		cout << "AÑADIENDO " << guardsNum << " guardias al nivel " << i << endl;
		for (int j = 0; j < guardsNum; ++j) 
		{
			cout << "  Añadiendo guardia en: " << guardsPositions[j].x << " " << guardsPositions[j].y << endl;
			level->addGuard(guardsPositions[j], texProgram);
		}

		// ROLLERS
		for (int j = 0; j < rollersNum; ++j)
			level->addRoller(rollersPositions[j], texProgram, true);

		// ARNOLD BOSS
		if (arnoldBoss)
			level->addArnoldBoss(arnoldPos, texProgram);

		// TWIN BOSS
		if (twin)
			level->addTwin(twinPos, texProgram);
	}

	fin.close();
	levelNum = 0;
	activeLevel = levels[levelNum];
}

void Scene::pause()
{
	pauseGame = true;
	player->setPause();
	activeLevel->setPause();
	characters->showText("transition");
	cout << "PAUSE" << endl;
}

void Scene::stop_pause()
{
	pauseGame = false;
	player->setStopPause();
	activeLevel->setStopPause();
	glm::ivec2 playerPos = player->getPosition();
	characters->hideText("transition");
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

bool Scene::isPaused() {
	return pauseGame;
}

void Scene::setLevelToGUI() {
	player->setLevelToGUI();
}

void Scene::collectAllObjects()
{
	// Primero, limpia el inventario del jugador
	player->clearAllObjects();

	// Regenera y recoge todos los objetos de todos los niveles
	for (int i = 0; i < levels.size(); i++)
	{
		Level* level = levels[i];

		// Regenera objetos (crea nuevos si fueron eliminados)
		level->regenerateWeapon(texProgram);
		level->regenerateAccessCard(texProgram);
		level->regenerateMeal(texProgram);

		// Añade al inventario
		if (level->hasWeapon())
		{
			player->addObject(level->getWeapon());
			level->hideObjectWeapon();
		}

		if (level->hasAccessCard())
		{
			player->addObject(level->getAccessCard());
			level->hideObjectAccessCard();
		}

		if (level->hasMeal())
		{
			player->addObject(level->getMeal());
			level->hideObjectMeal();
		}
	}

	// Actualiza el objeto activo
	player->setActiveObject();

	cout << "Todos los objetos recogidos. Total: " << player->getObjectCount() << endl;
}

void Scene::initGameOver()
{
	// Cargar la textura PNG
	gameOverTexture.loadFromFile("images/game_over.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Crear sprite del tamaño de la ventana
	glm::ivec2 spriteSize(640, 480);
	gameOverSprite = Sprite::createSprite(spriteSize, glm::vec2(1.0f, 1.0f), &gameOverTexture, &texProgram);

	// Posición (0,0) para que ocupe toda la pantalla
	gameOverSprite->setPosition(glm::vec2(0.f, 0.f));
}

// Modificar Scene::renderGameOver
void Scene::renderGameOver()
{
	if (!gameOverSprite) return;

	// Activar shader
	texProgram.use();

	// Renderizar el sprite del PNG a pantalla completa
	gameOverSprite->render();
}

void Scene::resetBullets()
{
	for (Level* level : levels)  // Recorre todos los niveles de la escena
	{
		for (Guard* guard : level->getGuards())  // Recorre todos los guardias del nivel
		{
			guard->clearBullets();  // Vacía las balas del guardia
		}
	}
}

void Scene::reset()
{
	std::cout << "[Scene] Reseteando escena..." << std::endl;

	// 1. Resetear jugador
	if (player) {
		TileMap* map = levels[3]->get_tile_map(); // Nivel inicial
		player->setTileMap(map);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
			INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->clearBullets(); // Limpiar balas del jugador
		player->reset();
	}

	// 2. Resetear todos los niveles
	for (Level* level : levels) {
		level->reset(); // Nueva función en Level
	}

	// 3. Volver al nivel inicial
	levelNum = 0;
	activeLevel = levels[levelNum];

	if (player) {
		TileMap* map = activeLevel->get_tile_map();
		player->setTileMap(map);
		player->setLevel(activeLevel);
	}

	// 4. Resetear variables de estado
	changingLevel = false;
	levelChangeDelay = 0.f;
	currentTime = 0.f;
	pauseGame = false;
	doorOpen = false;
	gameTime = 0.f;

	std::cout << "[Scene] Escena reseteada" << std::endl;
}