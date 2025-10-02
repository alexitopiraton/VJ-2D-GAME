#include "Menu.h"
#include "Game.h"
#include "ImageRenderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Menu::Menu()
{
    startPressed = false;
}

Menu::~Menu()
{
}

void Menu::init(ShaderProgram& shaderProgram)
{
    // Inicializar el ImageRenderer si no está ya inicializado
    ImageRenderer::instance().init(&shaderProgram);

    // Cargar la imagen del menú
    if (!ImageRenderer::instance().loadImage("menu", "images/menu.png")) {
        std::cout << "Error loading menu.png" << std::endl;
    }

    startPressed = false;
}

void Menu::update(int deltaTime)
{
    // Detectar si se presiona ENTER para empezar el juego
    if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE))
    {
        startPressed = true;
    }
}

void Menu::render()
{
    // Definir la región del menú europeo (cuadro negro sin bordes)
    ImageRegion menuRegion(0.35f, 0.65f, 0.05f, 0.45f);

    // Renderizar a pantalla completa
    ImageRenderer::instance().renderFullscreenRegion("menu", menuRegion);
}

void Menu::free()
{
    // El ImageRenderer se encarga de limpiar sus recursos
}

bool Menu::isStartPressed() const
{
    return startPressed;
}

void Menu::resetStartPressed()
{
    startPressed = false;
}