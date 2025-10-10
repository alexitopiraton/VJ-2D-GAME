#include "ScreenMap.h"
#include <iostream>

ScreenMap::ScreenMap()
{
    sprite = nullptr;
    posScreenMap = glm::vec2(0.0f, 0.0f);
}

ScreenMap::~ScreenMap()
{
    if (sprite != nullptr)
        delete sprite;
}

void ScreenMap::init(const glm::ivec2& imageSize, const glm::vec2& position, ShaderProgram& shaderProgram)
{
    posScreenMap = position;

    // Crear sprite con el tamaño de la imagen
    // glm::vec2(1.0, 1.0) significa que usa toda la textura (0.0 a 1.0 en coords de textura)
    sprite = Sprite::createSprite(imageSize, glm::vec2(1.0f, 1.0f), &mapImage, &shaderProgram);
    sprite->setNumberAnimations(1);

    // Una sola animación estática (la imagen completa)
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

    sprite->changeAnimation(0);
    sprite->setPosition(posScreenMap);
}

void ScreenMap::loadImage(const string& imagePath)
{
    mapImage.loadFromFile(imagePath, TEXTURE_PIXEL_FORMAT_RGBA);
    mapImage.setMinFilter(GL_NEAREST);
    mapImage.setMagFilter(GL_NEAREST);
}

void ScreenMap::render()
{
    if (sprite != nullptr) {
        sprite->render();
    }
}

void ScreenMap::setPosition(const glm::vec2& pos)
{
    posScreenMap = pos;
    if (sprite != nullptr)
        sprite->setPosition(posScreenMap);
}

glm::vec2 ScreenMap::getPosition() const
{
    return posScreenMap;
}