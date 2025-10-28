#include "Credits.h"
#include "SoundManager.h"
#include <iostream>

Credits::Credits()
{
    shaderProgram = nullptr;
    currentTime = 0.f;
    creditsFinished = false;
    scrollSpeed = 50.0f;
    scrollOffset = 0.f;
    musicStarted = false;
    creditsSprite = nullptr;
}

Credits::~Credits()
{
    if (creditsSprite != nullptr)
        delete creditsSprite;
}

void Credits::init(ShaderProgram& shaderProgram)
{
    this->shaderProgram = &shaderProgram;
    characters.init(shaderProgram);

    if (!creditsTexture.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        creditsSprite = nullptr;
    }
    else {
        creditsSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0f, 1.0f), &creditsTexture, this->shaderProgram);

        if (creditsSprite) {
            creditsSprite->setNumberAnimations(1);
            creditsSprite->setAnimationSpeed(0, 1);
            creditsSprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
            creditsSprite->changeAnimation(0);
            creditsSprite->setPosition(glm::vec2(0.f, 0.f));
        }
    }

    // Área de juego (el rectángulo blanco)
    // Basado en tu imagen, parece estar aproximadamente en:
    float gameAreaX = 10.f;      // Margen izquierdo
    float gameAreaY = 130.f;     // Debajo del HUD verde
    float gameAreaWidth = 620.f; // Ancho del área de juego
    float gameAreaHeight = 350.f; // Alto del área de juego

    float centerX = gameAreaX + (gameAreaWidth / 2.0f); // Centro del área de juego

    // Empezar desde abajo del área de juego
    float startY = gameAreaY + gameAreaHeight + 50.f;
    float lineSpacing = 40.f;

    // Función auxiliar para centrar texto dentro del área de juego
    auto centerText = [centerX](const std::string& text, float charWidth = 20.f) {
        return centerX - (text.length() * charWidth / 2.0f);
        };

    // Título del juego
    creditLines.push_back({ "line1", "METAL GEAR", glm::vec2(centerText("METAL GEAR"), startY), 0.f, false });
    startY += lineSpacing * 2;

    creditLines.push_back({ "line2", "A HIDEO KOJIMA GAME", glm::vec2(centerText("A HIDEO KOJIMA GAME"), startY), 0.5f, false });
    startY += lineSpacing * 2;

    // Game Design
    creditLines.push_back({ "line3", "PROGRAMMING", glm::vec2(centerText("PROGRAMMING"), startY), 1.0f, false });
    startY += lineSpacing;
    creditLines.push_back({ "line4", "ALEX URENA RIQUELME", glm::vec2(centerText("ALEX URENA RIQUELME"), startY), 1.5f, false });
    startY += lineSpacing * 2;

    // Programming
    creditLines.push_back({ "line5", "PROGRAMMING", glm::vec2(centerText("PROGRAMMING"), startY), 2.0f, false });
    startY += lineSpacing;
    creditLines.push_back({ "line6", "ROBERT MAS SORIA", glm::vec2(centerText("ROBERT MAS SORIA"), startY), 2.5f, false });
    startY += lineSpacing * 2;

    // Graphics
    creditLines.push_back({ "line7", "GRAPHICS", glm::vec2(centerText("GRAPHICS"), startY), 3.0f, false });
    startY += lineSpacing;
    creditLines.push_back({ "line8", "KONAMI 1987", glm::vec2(centerText("KONAMI 1987"), startY), 3.5f, false });
    startY += lineSpacing * 2;

    // Sound
    creditLines.push_back({ "line9", "SOUND", glm::vec2(centerText("SOUND"), startY), 4.0f, false });
    startY += lineSpacing;
    creditLines.push_back({ "line10", "KONAMI 1987", glm::vec2(centerText("KONAMI 1987"), startY), 4.5f, false });
    startY += lineSpacing * 2;

    // Thanks
    creditLines.push_back({ "line11", "THANKS FOR PLAYING", glm::vec2(centerText("THANKS FOR PLAYING"), startY), 5.0f, false });
    startY += lineSpacing * 2;

    creditLines.push_back({ "line12", "OPERATION COMPLETE", glm::vec2(centerText("OPERATION COMPLETE"), startY), 5.5f, false });
    startY += lineSpacing * 3;

    creditLines.push_back({ "line13", "THE END", glm::vec2(centerText("THE END"), startY), 6.0f, false });

    // Agregar todos los textos a Characters (inicialmente ocultos)
    for (const auto& line : creditLines)
    {
        characters.addText(line.id, line.text, line.initialPosition);
        characters.hideText(line.id);
    }

    currentTime = 0.f;
    scrollOffset = 0.f;
    creditsFinished = false;
    musicStarted = false;

    // Guardar los límites del área de juego para clipping
    this->gameAreaY = gameAreaY;
    this->gameAreaHeight = gameAreaHeight;
}

void Credits::render()
{
    if (!shaderProgram) return;

    shaderProgram->use();

    // Fondo negro
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
    shaderProgram->setUniformMatrix4f("projection", projection);
    shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    glm::mat4 modelview = glm::mat4(1.0f);
    shaderProgram->setUniformMatrix4f("modelview", modelview);
    shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

    if (creditsSprite)
        creditsSprite->render();

    // ? SIMPLE: Solo llamar a render de Characters
    characters.render();
}

void Credits::update(int deltaTime)
{
    currentTime += deltaTime / 1000.f;

    // Reproducir música de créditos
    if (!musicStarted)
    {
        SoundManager::instance().stopMusic();
        if (SoundManager::instance().loadMusic("sounds\\credits_music.mp3"))
        {
            SoundManager::instance().setMusicVolume(80);
            SoundManager::instance().playMusic(-1);
        }
        musicStarted = true;
    }

    if (creditsSprite)
        creditsSprite->update(deltaTime);

    // Scroll automático
    scrollOffset += scrollSpeed * (deltaTime / 1000.f);

    for (auto& line : creditLines)
    {
        if (currentTime >= line.delay && !line.visible)
        {
            line.visible = true;
            characters.showText(line.id);
        }

        if (line.visible)
        {
            glm::vec2 newPos = line.initialPosition;
            newPos.y -= scrollOffset;

            float topLimit = gameAreaY - 50.f;
            float bottomLimit = gameAreaY + gameAreaHeight / 2.0f + 150.f;
            float midScreen = gameAreaY + (gameAreaHeight / 2.0f);

            // Mostrar solo mientras está por debajo de la mitad
            if (newPos.y >= topLimit && newPos.y <= bottomLimit && newPos.y > midScreen)
            {
                characters.showText(line.id);
                characters.updatePosition(line.id, newPos);
            }
            else
            {
                characters.hideText(line.id);
            }
        }
    }

    if (!creditLines.empty())
    {
        const CreditLine& lastLine = creditLines.back();
        glm::vec2 lastPos = lastLine.initialPosition;
        lastPos.y -= scrollOffset;

        float midScreen = gameAreaY + (gameAreaHeight / 2.0f);

        // Terminar cuando la última línea pase la mitad
        if (lastLine.visible && lastPos.y < midScreen)
        {
            creditsFinished = true;
        }
    }

}

void Credits::reset()
{
    currentTime = 0.f;
    scrollOffset = 0.f;
    creditsFinished = false;
    musicStarted = false;

    // Resetear todas las líneas
    for (auto& line : creditLines)
    {
        line.visible = false;
        characters.hideText(line.id);
        characters.updatePosition(line.id, line.initialPosition);
    }
}