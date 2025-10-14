#include "Roller.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define ROLLER_SPEED 100.f  // píxeles por segundo
#define ROLLER_WIDTH 48
#define ROLLER_HEIGHT 16

enum RollerAnims {
    ROLL_LEFT, ROLL_RIGHT
};

Roller::Roller()
{
    sprite = nullptr;
    shaderProgram = nullptr;
    active = true;
    movingRight = true;
}

Roller::~Roller()
{
    if (sprite)
        delete sprite;
}

void Roller::init(const glm::vec2& mapDispl, ShaderProgram& shaderProgram, bool moveRight)
{
    this->mapDispl = mapDispl;
    this->shaderProgram = &shaderProgram;
    this->movingRight = moveRight;
    this->active = true;

    spritesheet.loadFromFile("images/enemies/roller.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(36 * 2, 144 * 2), glm::vec2(0.5f, 1.f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(ROLL_LEFT, 8);
    sprite->addKeyframe(ROLL_LEFT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(ROLL_LEFT, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(ROLL_RIGHT, 8);
    sprite->addKeyframe(ROLL_RIGHT, glm::vec2(0.f, 0.5f));
    sprite->addKeyframe(ROLL_RIGHT, glm::vec2(0.5f, 0.5f));

    sprite->changeAnimation(moveRight ? ROLL_RIGHT : ROLL_LEFT);
}

void Roller::setPosition(const glm::vec2& pos)
{
    this->pos = glm::ivec2(pos);
    sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void Roller::update(int deltaTime, TileMap* map, Player* player)
{
    if (!active) return;

    // Movimiento lateral
    float delta = (movingRight ? 1.0f : -1.0f) * ROLLER_SPEED * (deltaTime / 1000.0f);
    pos.x += delta;

    // Detectar colisión con paredes del TileMap
    int tileSize = map->getTileSize();
    if (pos.x < tileSize || pos.x > map->getMapSize().x * tileSize - ROLLER_WIDTH)
    {
        movingRight = !movingRight;
        sprite->changeAnimation(movingRight ? ROLL_RIGHT : ROLL_LEFT);
    }

    sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
    sprite->update(deltaTime);

    // --- Colisión con jugador ---
    glm::ivec2 playerPos = player->getPosition();
    glm::ivec2 playerSize(SPRITE_WIDTH, SPRITE_HEIGHT);

    bool collisionX = pos.x + ROLLER_WIDTH >= playerPos.x &&
        playerPos.x + playerSize.x >= pos.x;
    bool collisionY = pos.y + ROLLER_HEIGHT >= playerPos.y &&
        playerPos.y + playerSize.y >= pos.y;

    if (collisionX && collisionY)
    {
        std::cout << "[Roller] Jugador aplastado!" << std::endl;
        player->takeDamage(100);  // deberías tener este método (marca isDead = true)
        active = false;  // se desactiva al golpear
    }
}

void Roller::render()
{
    if (active && sprite)
        sprite->render();
}
