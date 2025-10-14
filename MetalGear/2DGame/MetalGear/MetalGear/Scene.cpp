#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include <GL/glew.h>
#include "Game.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Bullet.h"

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

	initGameOver();
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	if (player->isDead())
	{
		static float deathTimer = 0.f;
		deathTimer += deltaTime;
		if (deathTimer > 3000.f) { // 3 segundos
			resetBullets();
			player->reset();
			resetAll();
			for (Level* level : levels) {
				level->resetGuards();
				level->clearRollers();
			}
			Game::instance().returnToMenuFromGame();
			deathTimer = 0.f;
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
			cout << "Cooldown terminado" << endl;
		}
		else
		{
			player->update(deltaTime);
			return;
		}

	}
	
	player->update(deltaTime);
	activeLevel->update(deltaTime, player);

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
	renderHUD(player);

	if (player->isDead()) {
		renderGameOver();
	}
		
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




void Scene::renderHUD(Player* player)
{
	if (!player) return;

	float maxHP = 100.0f;
	float hp = static_cast<float>(player->getHealth());
	float ratio = hp / maxHP;

	float barWidth = 100.0f;
	float barHeight = 10.0f;
	float x = 20.0f;  // posición en pantalla
	float y = 20.0f;

	// ?? Desactivar shaders si los usas
	glUseProgram(0);

	// ?? Cambiar a modo 2D ortográfico (pantalla)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// ?? Dibujar barra
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	// Fondo gris
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(x, y);
	glVertex2f(x + barWidth, y);
	glVertex2f(x + barWidth, y + barHeight);
	glVertex2f(x, y + barHeight);

	// Vida verde
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(x, y);
	glVertex2f(x + barWidth * ratio, y);
	glVertex2f(x + barWidth * ratio, y + barHeight);
	glVertex2f(x, y + barHeight);

	glEnd();
	glEnable(GL_TEXTURE_2D);

	// ?? Restaurar matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

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

	if (levels.size() > 2)
	{
		TileMap* map = levels[0]->get_tile_map();

		glm::vec2 arnoldPos(5 * map->getTileSize(), 5 * map->getTileSize());
		levels[0]->addArnoldBoss(arnoldPos, texProgram);

		map = levels[2]->get_tile_map();


		// CAMBIA ESTAS COORDENADAS (10, 8) A DONDE QUIERAS EL GUARDIA
		glm::vec2 guardPos(10 * map->getTileSize(), 8 * map->getTileSize());
		levels[2]->addGuard(guardPos, texProgram);

		glm::vec2 guardPos2(15 * map->getTileSize(), 20 * map->getTileSize());
		levels[2]->addGuard(guardPos2, texProgram);

		cout << "Guardia agregado en level02 en posición tile (10, 8)" << endl;

		map = levels[3]->get_tile_map();

		glm::vec2 rollerPos(5 * map->getTileSize(), 22 * map->getTileSize());
		levels[3]->addRoller(rollerPos, texProgram, true);


		// Si quieres más guardias en el mismo nivel:
		// levels[2]->addGuard(glm::vec2(15 * map->getTileSize(), 12 * map->getTileSize()), texProgram);

		// Si quieres guardias en otros niveles:
		// TileMap* map3 = levels[3]->get_tile_map();
		// levels[3]->addGuard(glm::vec2(8 * map3->getTileSize(), 10 * map3->getTileSize()), texProgram);
	}
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

void Scene::resetAll()
{
	// Reset jugador
	TileMap* map = levels[0]->get_tile_map();
	player->reset();

	// Reset guardias y balas en TODOS los niveles
	for (Level* level : levels)
	{
		level->resetGuards();
	}

	// Reset variables de la escena
	changingLevel = false;
	levelChangeDelay = 0.f;
	currentTime = 0.f;
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



