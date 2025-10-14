#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <glm/glm.hpp>

class Bullet
{
public:
    Bullet(const glm::vec2& pos, const glm::vec2& dir, ShaderProgram* shaderProgram);

    void update(int deltaTime, TileMap* tilemap);
    void render();

    bool isAlive() const { return alive; }
    glm::vec2 getPosition() const { return position; }
    void setAlive(bool state) { alive = state; }


private:
    Sprite* sprite;
    Texture texture;
    glm::vec2 position;
    glm::vec2 direction;
    float speed = 200.0f; // píxeles por segundo
    bool alive;
};

#endif // _BULLET_INCLUDE
#pragma once
