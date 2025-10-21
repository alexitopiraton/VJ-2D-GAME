#include "Menu.h"
#include "Game.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

Menu::Menu()
{
    startPressed = false;
    menuSprite = nullptr;
    cursorSprite = nullptr;
    shaderProgram = nullptr;
    selectedOption = 0; // 0 = START, 1 = CONTINUE
}

Menu::~Menu()
{
    if (menuSprite != nullptr)
        delete menuSprite;
    if (cursorSprite != nullptr)
        delete cursorSprite;
}

void Menu::init(ShaderProgram& shaderProgram)
{
    this->shaderProgram = &shaderProgram;

    std::cout << "Inicializando menú..." << std::endl;

    // ? Verificar que el shader está correctamente linkeado
    if (!shaderProgram.isLinked()) {
        std::cout << "ERROR: ShaderProgram no está linkeado correctamente" << std::endl;
        return;
    }

    // ====== CARGAR IMAGEN DEL MENÚ ======
    std::cout << "Cargando menu.png..." << std::endl;
    if (!menuTexture.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cout << "ERROR: No se pudo cargar images/menu.png" << std::endl;
        return;
    }
    std::cout << "Menu.png cargado: " << menuTexture.width() << "x" << menuTexture.height() << std::endl;

    menuSprite = Sprite::createSprite(
        glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT),
        glm::vec2(1.0f, 1.0f),
        &menuTexture,
        this->shaderProgram
    );

    if (!menuSprite) {
        std::cout << "ERROR: No se pudo crear menuSprite" << std::endl;
        return;
    }

    menuSprite->setNumberAnimations(1);
    menuSprite->setAnimationSpeed(0, 1);
    menuSprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
    menuSprite->changeAnimation(0);
    menuSprite->setPosition(glm::vec2(0.f, 0.f));

    std::cout << "Menu sprite creado correctamente" << std::endl;

    // ====== CARGAR SPRITE DEL CURSOR (PISTOLA) ======
    std::cout << "Cargando cursor.png..." << std::endl;
    if (!cursorTexture.loadFromFile("images/menuGun.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cout << "ADVERTENCIA: No se pudo cargar images/cursor.png" << std::endl;
        std::cout << "El menú funcionará sin cursor" << std::endl;
        cursorSprite = nullptr; // Continuar sin cursor
    }
    else {
        std::cout << "Cursor.png cargado: " << cursorTexture.width() << "x" << cursorTexture.height() << std::endl;

        // Tamaño del cursor escalado
        int cursorSize = 32; // Aumentado para que se vea mejor
        cursorSprite = Sprite::createSprite(
            glm::ivec2(cursorSize, cursorSize),
            glm::vec2(1.0f, 1.0f),
            &cursorTexture,
            this->shaderProgram
        );

        if (cursorSprite) {
            cursorSprite->setNumberAnimations(1);
            cursorSprite->setAnimationSpeed(0, 1);
            cursorSprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
            cursorSprite->changeAnimation(0);

            // Posiciones del cursor (ajusta según tu menú)
            startPosition = glm::vec2(210.f, 270.f);    // Ajusta estas posiciones
            continuePosition = glm::vec2(210.f, 300.f); // Ajusta estas posiciones

            updateCursorPosition();
            std::cout << "Cursor inicializado correctamente" << std::endl;
        }
    }

    startPressed = false;
    selectedOption = 0;

    // --- Configuración del parpadeo ---
    blinkTime = 0.f;
    blinkInterval = 500.f; 
    cursorVisible = true;

    std::cout << "Menu inicializado completamente" << std::endl;
}

void Menu::update(int deltaTime)
{
    static bool upPressed = false;
    static bool downPressed = false;

    // ====== NAVEGACIÓN CON FLECHAS (solo visual) ======
    if (Game::instance().getKey(GLFW_KEY_UP))
    {
        if (!upPressed) {
            selectedOption = 0; // START
            updateCursorPosition();
            upPressed = true;
        }
    }
    else {
        upPressed = false;
    }

    if (Game::instance().getKey(GLFW_KEY_DOWN))
    {
        if (!downPressed) {
            selectedOption = 1; // CONTINUE
            updateCursorPosition();
            downPressed = true;
        }
    }
    else {
        downPressed = false;
    }

    // ====== SELECCIÓN CON ENTER/ESPACIO (solo funciona en START) ======
    if (Game::instance().getKey(GLFW_KEY_ENTER) || Game::instance().getKey(GLFW_KEY_SPACE))
    {
        // Solo activar si está en START
        if (selectedOption == 0) {
            startPressed = true;
        }
    }

    // Actualizar sprites
    if (menuSprite)
        menuSprite->update(deltaTime);
    if (cursorSprite)
        cursorSprite->update(deltaTime);

    blinkTime += deltaTime;
    if (blinkTime >= blinkInterval) {
        blinkTime = 0.f;
        cursorVisible = !cursorVisible; 
    }
}

void Menu::render()
{
    if (!shaderProgram) return;

    // Configurar shader
    shaderProgram->use();
    glm::mat4 projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
    shaderProgram->setUniformMatrix4f("projection", projection);
    shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    glm::mat4 modelview = glm::mat4(1.0f);
    shaderProgram->setUniformMatrix4f("modelview", modelview);
    shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

    // Renderizar menú
    if (menuSprite)
        menuSprite->render();

  
    // Renderizar cursor (pistola)
    if (cursorSprite && cursorVisible)
        cursorSprite->render();
}

void Menu::updateCursorPosition()
{
    if (!cursorSprite) return;

    if (selectedOption == 0) {
        cursorSprite->setPosition(startPosition);
    }
    else {
        cursorSprite->setPosition(continuePosition);
    }
}

void Menu::free()
{
    if (menuSprite != nullptr) {
        delete menuSprite;
        menuSprite = nullptr;
    }
    if (cursorSprite != nullptr) {
        delete cursorSprite;
        cursorSprite = nullptr;
    }
}

bool Menu::isStartPressed() const
{
    return startPressed;
}

int Menu::getSelectedOption() const
{
    return selectedOption;
}

void Menu::resetStartPressed()
{
    startPressed = false;
}