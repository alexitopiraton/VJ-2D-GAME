#include "Roller.h"
#include "Player.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define ROLLER_SPEED 350.f  
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

void Roller::init(const glm::vec2& mapDispl, ShaderProgram& shaderProgram, bool moveRight, const int& tileSize)
{
    this->mapDispl = mapDispl;
    this->shaderProgram = &shaderProgram;
    this->movingRight = moveRight;
    this->active = true;

    spritesheet.loadFromFile("images/enemies/roller.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(2 * tileSize, 2 * tileSize), glm::vec2(0.5f, 0.25f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(ROLL_LEFT, 8);
    sprite->addKeyframe(ROLL_LEFT, glm::vec2(0.0f, 0.75f));
    sprite->addKeyframe(ROLL_LEFT, glm::vec2(0.5f, 0.75f));

    sprite->setAnimationSpeed(ROLL_RIGHT, 8);
    sprite->addKeyframe(ROLL_RIGHT, glm::vec2(0.5f, 0.0f));
    sprite->addKeyframe(ROLL_RIGHT, glm::vec2(0.0f, 0.0f));

    sprite->changeAnimation(moveRight ? ROLL_RIGHT : ROLL_LEFT);

    this->movingRight = moveRight;
    this->initialMovingRight = moveRight; // Guardar estado inicial
    this->active = true;
}


void Roller::setPosition(const glm::vec2& pos)
{
    this->pos = glm::ivec2(pos);
    this->initialPos = pos; // Guardar posición inicial
    sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void Roller::update(int deltaTime, TileMap* map, Player* player)
{
    if (!active) return;

    int tileSize = map->getTileSize();

    // Calcular nueva posición
    float delta = (movingRight ? 1.0f : -1.0f) * ROLLER_SPEED * (deltaTime / 1000.0f);
    glm::ivec2 newPos = pos;
    newPos.x += delta;

    // --- Verificar colisión con paredes usando isWalkable ---

    // Calcular los tiles que ocuparía en la nueva posición
    glm::ivec2 frontTile;

    if (movingRight)
    {
        // Verificar tile del borde derecho
        frontTile = map->worldToTileCoords(glm::vec2(newPos.x + ROLLER_WIDTH, newPos.y + ROLLER_HEIGHT / 2));
    }
    else
    {
        // Verificar tile del borde izquierdo
        frontTile = map->worldToTileCoords(glm::vec2(newPos.x, newPos.y + ROLLER_HEIGHT / 2));
    }

    // Si el tile frontal NO es caminable, rebotar
    if (!map->isWalkable(frontTile.x, frontTile.y))
    {
        // Cambiar dirección
        movingRight = !movingRight;
        sprite->changeAnimation(movingRight ? ROLL_RIGHT : ROLL_LEFT);

        // No actualizar posición (mantener la actual)
    }
    else
    {
        // Si es caminable, actualizar posición
        pos = newPos;
    }

    // --- Verificar también los bordes del mapa como seguridad ---
    int mapWidth = map->getMapSize().x * tileSize;

    if (pos.x <= tileSize)
    {
        pos.x = tileSize;
        movingRight = true;
        sprite->changeAnimation(ROLL_RIGHT);
    }
    else if (pos.x + ROLLER_WIDTH >= mapWidth - tileSize)
    {
        pos.x = mapWidth - tileSize - ROLLER_WIDTH;
        movingRight = false;
        sprite->changeAnimation(ROLL_LEFT);
    }

    // Actualizar sprite
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
        player->takeDamage(100);
        active = false;
    }
}


void Roller::render()
{
    if (active && sprite)
        sprite->render();
}

void Roller::reset()
{
    // Volver a posición inicial
    pos = initialPos;
    sprite->setPosition(glm::vec2(float(initialPos.x), float(initialPos.y)));

    // Volver a dirección inicial
    movingRight = initialMovingRight;
    sprite->changeAnimation(movingRight ? ROLL_RIGHT : ROLL_LEFT);

    // Reactivar
    active = true;
}