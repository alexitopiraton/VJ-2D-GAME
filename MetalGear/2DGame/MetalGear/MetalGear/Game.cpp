#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	currentState = MENU; // Empezar en el menú
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	// Inicializar el menú y la escena del juego
	scene.init();
	menu.init(scene.getTexProgram()); // Necesitaremos modificar Scene para obtener el shader program
	gui.init(scene.getTexProgram());
	
	scene.getPlayer()->setGui(&gui);
	gui.setMaxHealth(100);
	gui.updateHealth(75);
	scene.getPlayer()->setHealth(75);
}

bool Game::update(int deltaTime)
{
	switch (currentState)
	{
	case MENU:
		menu.update(deltaTime);
		if (menu.isStartPressed())
		{
			currentState = PLAYING;
			menu.resetStartPressed();
		}
		break;

	case PLAYING:
		scene.update(deltaTime);
		gui.update(deltaTime);
		// Puedes agregar lógica para volver al menú si se presiona ESC
		break;

	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (currentState)
	{
	case MENU:
		glViewport(0, 0, SCREEN_FINAL_WIDTH, SCREEN_FINAL_HEIGHT);
		menu.render();
		break;

	case PLAYING:
		if (!scene.isPaused())
		{
			glViewport(0, GUI_HEIGHT, GAME_WIDTH, GAME_HEIGHT);
			scene.render();
			scene.setLevelToGUI();
			glViewport(0, 0, GUI_WIDTH, GUI_HEIGHT);
			gui.render();
		}
		else
		{
			glViewport(0, 0, SCREEN_FINAL_WIDTH, SCREEN_FINAL_HEIGHT);
			scene.render();
		}

		break;

	}
}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE) // Escape code
	{
		if (currentState == PLAYING)
		{
			currentState = MENU; // Volver al menú desde el juego
		}
		else
		{
			bPlay = false; // Salir del juego desde el menú
		}
	}
	keys[key] = true;
}

void Game::returnToMenuFromGame()
{
	init(); // Reiniciar el juego
	//currentState = MENU;
	//menu.resetStartPressed();  // para evitar reinicios automáticos
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}