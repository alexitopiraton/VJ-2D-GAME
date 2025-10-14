#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <cmath>
#include <iostream>

Bullet::Bullet(const glm::vec2& pos, const glm::vec2& dir, ShaderProgram* shaderProgram)
{
    texture.loadFromFile("images/bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(16,16), glm::vec2(1.0f, 1.0f), &texture, shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);

    position = pos;
    direction = glm::normalize(dir);
    sprite->setPosition(position);
}

void Bullet::update(int deltaTime, TileMap* tilemap)
{
    std::cout << "[Bullet] pos=" << position.x << "," << position.y
        << " alive=" << alive << std::endl;

    std::cout << "[Bullet::update] alive=" << alive
        << " tilemap=" << tilemap << std::endl;


    if (!alive) return;
    if (!tilemap) return;
    float step = speed * (deltaTime / 1000.0f);
    glm::vec2 newPos = position + direction * step;

    std::cout << "[Bullet] pos=" << position.x << "," << position.y
        << " dir=" << direction.x << "," << direction.y
        << " step=" << step << std::endl;


    // comprobar colisión con paredes
    glm::ivec2 tile = tilemap->worldToTileCoords(newPos);
    if (!tilemap->isWalkable(tile.x, tile.y)) {
        alive = false;
        return;
    }

    position = newPos;
    sprite->setPosition(position);
}

void Bullet::render()
{
    if (alive && sprite) {
        sprite->render();
    }
}

