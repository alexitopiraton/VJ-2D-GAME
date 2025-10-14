#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Guard.h"
#include "Game.h"
#include "Pathfinder.h"
#include "Bullet.h"

enum GuardAnims
{
    STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Guard::init(ShaderProgram& shaderProgram)
{

    this->shaderProgram = &shaderProgram;
    // Usa el mismo spritesheet que el jugador temporalmente para probar
    spritesheet.loadFromFile("images/enemies/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // Usar las mismas dimensiones y animaciones que el jugador por ahora
    sprite = Sprite::createSprite(glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), glm::vec2(0.2f, 0.5f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(STAND_LEFT, 8);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(SPRITESHEET_OFFSET * 4, 0.5f));

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5, 0.5f));

    sprite->setAnimationSpeed(MOVE_LEFT, 5);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(SPRITESHEET_OFFSET * 0, 0.5f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(SPRITESHEET_OFFSET * 2, 0.5f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 5);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 1, 0.5f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 3, 0.5f));

    sprite->changeAnimation(STAND_RIGHT);
    sprite->setPosition(glm::vec2(float(posGuard.x), float(posGuard.y)));

    // Inicializar variables de pathfinding
    currentPathIndex = 0;
    lastPlayerTile = glm::ivec2(-1, -1);
    timeSinceLastPath = 0;
}

void Guard::shootAtPlayer(Player& player)
{
	if (player.isDead()) return;
    if (timeSinceLastShot < fireCooldown)
        return; // aún no puede disparar

    timeSinceLastShot = 0; // reinicia cooldown

    glm::vec2 playerPos = player.getPosition();
    glm::vec2 direction = glm::normalize(playerPos - posGuard);

    // Crear la bala
    Bullet* bullet = new Bullet(posGuard + glm::vec2(16, 16), direction, shaderProgram);
    bullet->setAlive(true);
    bullets.push_back(bullet);

    if (direction.x < 0)
        sprite->changeAnimation(STAND_LEFT);
    else
        sprite->changeAnimation(STAND_RIGHT);


    std::cout << "[Guard] Disparo hacia el jugador! dirección=("
        << direction.x << ", " << direction.y << ")" << std::endl;

    std::cout << "[DEBUG] bullets.size() = " << bullets.size() << std::endl;
}


void Guard::AIControl(TileMap& tilemap, Player& player, int deltaTime)
{
    timeSinceLastShot += deltaTime;
	cout << "[Guard] AIControl called. timeSinceLastShot=" << timeSinceLastShot << endl;

    this->map = &tilemap;

    glm::vec2 guardPos = posGuard + glm::vec2(16, 16);
    glm::vec2 playerPos = glm::vec2(player.getPosition()) + glm::vec2(16, 16);

    float dist = glm::distance(guardPos, playerPos);
    float attackRadiusPixels = 150.0f; // por ejemplo, 150 px
	cout << "[Guard] Distancia al jugador: " << dist << " tiles." << endl;

    // Si el jugador está dentro del radio de ataque
    if (dist <= attackRadiusPixels)
    {
		cout << "[Guard] Jugador en rango de ataque (" << attackRadiusPixels << " tiles). Disparando!" << endl;
        shootAtPlayer(player);
    }
    else
    {
        // fuera del rango: idle (no hace nada)
        // podrías poner una animación o mirar hacia el jugador
    }
}


bool Guard::moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime)
{
    const float speedPixelsPerSecond = 60.0f;
    const float threshold = 2.0f; // Píxeles de tolerancia para considerar que llegó

    // Calcular posición objetivo en píxeles (centro del tile)
    glm::vec2 targetWorld = tilemap.tileToWorldCoords(nextTile);

    // Centrar el sprite en el tile
    float offsetX = (tilemap.getTileSize() - 32) * 0.5f;
    float offsetY = (tilemap.getTileSize() - 62) * 0.5f; // Altura del sprite ajustada
    targetWorld += glm::vec2(offsetX, offsetY);

    std::cout << "[moveTowardsTile] posGuard=(" << posGuard.x << "," << posGuard.y
        << ") target=(" << targetWorld.x << "," << targetWorld.y << ")" << std::endl;

    // Calcular dirección y distancia
    glm::vec2 dir = targetWorld - glm::vec2(posGuard);
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

    std::cout << "[moveTowardsTile] distancia=" << dist << " threshold=" << threshold << std::endl;

    // Si ya llegó al tile
    if (dist < threshold)
    {
        std::cout << "[moveTowardsTile] LLEGÓ AL TILE!" << std::endl;
        posGuard = glm::ivec2(targetWorld);
        sprite->setPosition(glm::vec2(posGuard));
        return true;
    }

    // Calcular movimiento
    float step = speedPixelsPerSecond * (deltaTime / 1000.0f);
    glm::vec2 movement = (dist <= step) ? dir : (dir / dist) * step;

    std::cout << "[moveTowardsTile] step=" << step << " movement=("
        << movement.x << "," << movement.y << ")" << std::endl;

    // Actualizar animación según la dirección
    if (abs(movement.x) > abs(movement.y))
    {
        if (movement.x > 0)
            sprite->changeAnimation(MOVE_RIGHT);
        else
            sprite->changeAnimation(MOVE_LEFT);
    }

    // Mover el guardia
    posGuard += glm::ivec2(movement);
    sprite->setPosition(glm::vec2(posGuard));

    std::cout << "[moveTowardsTile] nueva pos=(" << posGuard.x << "," << posGuard.y << ")" << std::endl;

    return false;
}

void Guard::update(int deltaTime)
{
    sprite->update(deltaTime);
    // Actualizar balas
    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        (*it)->update(deltaTime, map);
        if (!(*it)->isAlive()) {
            delete* it;
            it = bullets.erase(it);
        }
        else ++it;
    }
}

void Guard::render()
{
    sprite->render();
    for (Bullet* b : bullets)
        b->render();
}

void Guard::setPosition(const glm::vec2& pos)
{
    posGuard = glm::ivec2(pos);
    sprite->setPosition(glm::vec2(float(posGuard.x), float(posGuard.y)));
}

void Guard::setShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
}

void Guard::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;

    std::cout << "[Guard] Recibió daño! Vida actual: " << health << std::endl;

    if (health <= 0)
    {
        std::cout << "[Guard] Muerto!" << std::endl;
        // Aquí podrías eliminar el sprite, desactivar el guardia, etc.
    }
}

void Guard::clearBullets()
{
    for (Bullet* b : bullets)
        delete b; // liberar memoria si las balas se crean con new
    bullets.clear();
}

void Guard::reset()
{
    alive = true;
    clearBullets();
	health = 50;    
    currentPath.clear();
    currentPathIndex = 0;
    lastPlayerTile = glm::ivec2(-1, -1);
	timeSinceLastPath = 0;
    timeSinceLastShot = fireCooldown; // puede disparar inmediatamente
	sprite->changeAnimation(STAND_RIGHT);
}
