#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	currentState = MENU; // Empezar en el men�
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	// Inicializar el men� y la escena del juego
	scene.init();
	menu.init(scene.getTexProgram()); // Necesitaremos modificar Scene para obtener el shader program
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
		// Puedes agregar l�gica para volver al men� si se presiona ESC
		break;

	case PAUSED:
		// L�gica para el estado de pausa si la necesitas
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
		menu.render();
		break;

	case PLAYING:
		scene.render();
		break;

	case PAUSED:
		// Renderizar pantalla de pausa
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE) // Escape code
	{
		if (currentState == PLAYING)
		{
			currentState = MENU; // Volver al men� desde el juego
		}
		else
		{
			bPlay = false; // Salir del juego desde el men�
		}
	}
	keys[key] = true;
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