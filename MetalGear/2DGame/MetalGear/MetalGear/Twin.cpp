#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <cstdlib>
#include <ctime>
#include "Twin.h"
#include "Game.h"
#include "Player.h"
#include "Pathfinder.h"
#include "Bullet.h"

enum TwinAnims
{
    STAND_LEFT, STAND_RIGHT
};

void Twin::init(ShaderProgram& shaderProgram)
{
    this->shaderProgram = &shaderProgram;
    spritesheet.loadFromFile("images/enemies/twin.png", TEXTURE_PIXEL_FORMAT_RGBA);

    sprite = Sprite::createSprite(glm::ivec2(SPRITE_WIDTH + 30, SPRITE_HEIGHT + 10), glm::vec2(0.5f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(STAND_LEFT, 8);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));

    sprite->changeAnimation(STAND_RIGHT);
    sprite->setPosition(glm::vec2(float(posTwin.x), float(posTwin.y)));

    movingDown = true;
    verticalSpeed = 120.0f;

    animationTimer = 0;
    animationSwitchTime = 300;

    fireCooldown = 200; 

    sound = SoundManager::instance().loadSound("sounds/bullet_sound.wav");

    srand(static_cast<unsigned int>(time(nullptr)));
}

void Twin::shootAtPlayer(Player& player)
{
    if (player.getIsDead()) return;
    if (timeSinceLastShot < fireCooldown)
        return;

    if (alive)
    {
        timeSinceLastShot = 0;

        glm::vec2 playerPos = player.getPosition();
        glm::vec2 direction = glm::normalize(playerPos - posTwin);

        Bullet* bullet = new Bullet(posTwin + glm::vec2(16, 16), direction, shaderProgram, BulletType::TWIN);
        bullet->setAlive(true);
        bullets.push_back(bullet);
        SoundManager::instance().playSound(sound, 0);
    }
}

void Twin::AIControl(TileMap& tilemap, Player& player, int deltaTime)
{
    timeSinceLastShot += deltaTime;
    this->map = &tilemap;

    this->targetPlayer = &player;

    shootAtPlayer(player);
}

void Twin::updateVerticalMovement(int deltaTime, TileMap& tilemap)
{
    if (!map || !targetPlayer) return;

    glm::vec2 playerPos = targetPlayer->getPosition();

    float step = verticalSpeed * (deltaTime / 1000.0f);
    glm::vec2 newPos = posTwin;

    // Seguir al jugador solo en Y
    if (fabs(playerPos.y - posTwin.y) > 2.0f) { // margen pequeño
        if (playerPos.y > posTwin.y)
            newPos.y += step;
        else
            newPos.y -= step;
    }

    // Comprobar colisión solo en tiles relevantes
    int tileSize = tilemap.getTileSize();
    glm::ivec2 midTile = tilemap.worldToTileCoords(newPos + glm::vec2(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2));

    if (tilemap.isWalkable(midTile.x, midTile.y)) {
        posTwin = newPos;
        sprite->setPosition(posTwin);
    }

    // Alternar animación (para simular movimiento)
    animationTimer += deltaTime;
    if (animationTimer >= animationSwitchTime)
    {
        animationTimer = 0;
        if (sprite->animation() == STAND_LEFT)
            sprite->changeAnimation(STAND_RIGHT);
        else
            sprite->changeAnimation(STAND_LEFT);
    }
}


void Twin::update(int deltaTime)
{
    if (alive)
    {
        // Actualizar movimiento vertical
        if (map)
            updateVerticalMovement(deltaTime, *map);

        // Actualizar sprite
        sprite->update(deltaTime);

        // ? Actualizar y limpiar balas de forma más eficiente
        for (auto it = bullets.begin(); it != bullets.end(); )
        {
            Bullet* bullet = *it;
            bullet->update(deltaTime, map);

            // Eliminar balas que ya no están vivas O que salieron muy lejos de la pantalla
            if (!bullet->isAlive())
            {
                delete bullet;
                it = bullets.erase(it);
            }
            else
            {
                // ? IMPORTANTE: Eliminar balas que se fueron muy lejos
                glm::vec2 bulletPos = bullet->getPosition();
                float distanceFromTwin = glm::length(bulletPos - posTwin);

                // Si la bala está a más de 800 píxeles, eliminarla
                if (distanceFromTwin > 800.0f)
                {
                    bullet->setAlive(false);
                    delete bullet;
                    it = bullets.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        // ? Limitar el número máximo de balas activas
        const int MAX_BULLETS = 20;
        while (bullets.size() > MAX_BULLETS)
        {
            Bullet* oldest = bullets.front();
            delete oldest;
            bullets.pop_front();
        }
    }
}

void Twin::render()
{
    if (alive) 
    {
        sprite->render();

        // Renderizar balas
        for (Bullet* b : bullets)
        {
            b->render();
        }
    }
}

void Twin::setPosition(const glm::vec2& pos)
{
    posTwin = glm::ivec2(pos);
    sprite->setPosition(glm::vec2(float(posTwin.x), float(posTwin.y)));
}

void Twin::setShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
}

void Twin::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;

    if (health <= 0)
    {
        alive = false;
    }
}

void Twin::clearBullets()
{
    for (Bullet* b : bullets)
        delete b;
    bullets.clear();
}

void Twin::reset()
{
    alive = true;
    clearBullets();
    health = 50;
    timeSinceLastShot = 0;
    movingDown = true;
    sprite->changeAnimation(STAND_RIGHT);
}