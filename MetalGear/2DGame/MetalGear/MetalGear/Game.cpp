#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	currentState = MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	SoundManager::instance().init();

	scene.init();
	menu.init(scene.getTexProgram());
	gui.init(scene.getTexProgram());
	credits.init(scene.getTexProgram());
	
	scene.getPlayer()->setGui(&gui);
	gui.setMaxHealth(100);
	gui.updateHealth(100);
	scene.getPlayer()->setHealth(100);

	arnoldDead = false;

	if (SoundManager::instance().loadMusic("sounds/menu_music.mp3"))
	{
		SoundManager::instance().setMusicVolume(64);
		SoundManager::instance().playMusic(-1);
	}
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

			SoundManager::instance().stopMusic();
			if (SoundManager::instance().loadMusic("sounds/game_jungle_music.mp3"))
			{
				SoundManager::instance().playMusic(-1);
			}

		}
		break;

	case PLAYING:
		scene.update(deltaTime);
		gui.update(deltaTime);
		break;

	case CREDITS:
		credits.update(deltaTime);

		if (credits.isFinished())
		{
			returnToMenuFromGame();
			currentState = MENU;
			SoundManager::instance().stopMusic();
			if (SoundManager::instance().loadMusic("sounds/menu_music.mp3"))
			{
				SoundManager::instance().playMusic(-1);
			}	
		}
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

	case CREDITS:
		credits.render();
		break;

	}
}

void Game::winGame()
{
	glViewport(0, 0, SCREEN_FINAL_WIDTH, SCREEN_FINAL_HEIGHT);
	currentState = CREDITS;
	credits.reset();

}

void Game::keyPressed(int key)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		if (currentState == PLAYING)
		{
			currentState = MENU; 
			
			SoundManager::instance().stopMusic();
			if (SoundManager::instance().loadMusic("sounds/menu_music.mp3"))
			{
				SoundManager::instance().playMusic(-1);
			}
		}
		else
		{
			bPlay = false;
		}
	}
	keys[key] = true;
}

void Game::resetGame()
{
	gui.reset();
	scene.reset();

	scene.getPlayer()->setGui(&gui);
	gui.updateHealth(100);
	gui.updateHealth(100);
	scene.getPlayer()->setHealth(100);
}

void Game::returnToMenuFromGame()
{	
	resetGame();
	currentState = MENU;
	SoundManager::instance().stopMusic();
	if (SoundManager::instance().loadMusic("sounds/menu_music.mp3"))
	{
		SoundManager::instance().playMusic(-1);
	}
	menu.resetStartPressed();
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