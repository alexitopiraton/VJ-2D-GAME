#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Player.h"
#include "Level.h"
#include <vector>

#define LEVEL_CHANGE_COOLDOWN 2000.0f

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{
public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void initialise_levels();
	void pause();
	void stop_pause();

	// Getter para acceder al shader program desde otras clases
	ShaderProgram& getTexProgram() { return texProgram; }

private:
	void initShaders();

private:
	Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	vector<Level*> levels;
	int levelNum;
	Level* activeLevel;

	bool changingLevel;
	float levelChangeDelay;
	bool pauseGame;
	float gameTime;
};

#endif // _SCENE_INCLUDE