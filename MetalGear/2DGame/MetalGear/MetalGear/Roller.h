#pragma once
#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "ShaderProgram.h"

class Roller
{
public:
    Roller();
    ~Roller();

    void init(const glm::vec2& mapDispl, ShaderProgram& shaderProgram, bool moveRight);
    void update(int deltaTime, TileMap* map, Player* player);
    void render();

    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const { return pos; }

    bool isActive() const { return active; }
    void deactivate() { active = false; }

private:
    glm::ivec2 pos;
    glm::vec2 speed;
    glm::vec2 mapDispl;
    bool movingRight;
    bool active;

    Texture spritesheet;
    Sprite* sprite;
    ShaderProgram* shaderProgram;
};
