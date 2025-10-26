#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Menu.h"
#include "GameState.h"
#include "Gui.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// SCALED SCREEN RESOLUTION (4:3). GAME RESOLUTION
#define SCALE 1.5
#define GAME_WIDTH (SCREEN_WIDTH * SCALE)
#define GAME_HEIGHT (SCREEN_HEIGHT * SCALE)

// GUI RESOLUTION
#define GUI_WIDTH GAME_WIDTH
#define GUI_HEIGHT 40 * 3

// TOTAL SCREEN DIMENSIONS
#define SCREEN_FINAL_WIDTH GAME_WIDTH
//#define SCREEN_FINAL_HEIGHT GAME_HEIGHT
#define SCREEN_FINAL_HEIGHT (GAME_HEIGHT + GUI_HEIGHT)



// Game is a singleton (a class with a single instance) that represents our whole application

class Game
{
private:
	Game() {}

public:
	static Game& instance()
	{
		static Game G;
		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	void returnToMenuFromGame();
	void resetGame();

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST + 1]; // Store key states so that 
	// we can have access at any time
	GameState currentState;
	Scene scene;
	Menu menu;
	Gui gui;
};

#endif // _GAME_INCLUDE