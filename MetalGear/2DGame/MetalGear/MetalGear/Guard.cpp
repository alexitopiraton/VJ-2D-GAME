#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Guard.h"
#include "Game.h"
#include "Pathfinder.h"
#include "Bullet.h"
#include "Player.h"
#include <cstdlib>
#include <ctime>

#define PATROL_SPEED 50.0f      // ? ra 30.0f)
#define IDLE_TIME_MIN 500       // ? Menos tiempo parado (era 1000)
#define IDLE_TIME_MAX 1500      // ? Menos tiempo parado (era 3000)
#define PATROL_TIME_MIN 800     // ? Menos tiempo caminando (era 1500)
#define PATROL_TIME_MAX 2000    // ? Menos tiempo caminando (era 4000)
#define SHOOT_COOLDOWN 800      // ?)
#define DETECTION_RANGE 500.0f  

enum GuardAnims
{
    STAND_LEFT, STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN
};

void Guard::init(ShaderProgram& shaderProgram)
{
    this->shaderProgram = &shaderProgram;
    spritesheet.loadFromFile("images/enemies/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);

    sprite = Sprite::createSprite(glm::ivec2(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(8);

    sprite->setAnimationSpeed(STAND_LEFT, 8);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.5f));

    sprite->setAnimationSpeed(STAND_UP, 8);
    sprite->addKeyframe(STAND_UP, glm::vec2(0.25f, 0.0f));

    sprite->setAnimationSpeed(STAND_DOWN, 8);
    sprite->addKeyframe(STAND_DOWN, glm::vec2(0.0f, 0.0f));

    sprite->setAnimationSpeed(MOVE_LEFT, 6);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 6);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.5f));

    sprite->setAnimationSpeed(MOVE_UP, 6);
    sprite->addKeyframe(MOVE_UP, glm::vec2(0.25f, 0.0f));
    sprite->addKeyframe(MOVE_UP, glm::vec2(0.75f, 0.0f));

    sprite->setAnimationSpeed(MOVE_DOWN, 6);
    sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.0f, 0.0f));
    sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.5f, 0.0f));

    sprite->changeAnimation(STAND_RIGHT);
    sprite->setPosition(glm::vec2(float(posGuard.x), float(posGuard.y)));

    alertTexture.loadFromFile("images/alert.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spriteAlert = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0f, 1.0f), &alertTexture, &shaderProgram);
    spriteAlert->setNumberAnimations(1);
    spriteAlert->setAnimationSpeed(0, 1);
    spriteAlert->addKeyframe(0, glm::vec2(0.f, 0.f));
    spriteAlert->changeAnimation(0);

    state = GUARD_IDLE;
    currentDirection = NONE;
    facingDirection = RIGHT;
    moveSpeed = PATROL_SPEED;

    idleTimer = 0;
    patrolTimer = 0;
    directionChangeTimer = 500;

    showAlert = false;
    playerDetected = false;

    map = nullptr;
    targetPlayer = nullptr;

    srand(static_cast<unsigned int>(time(nullptr)));
}


void Guard::AIControl(TileMap& tilemap, Player& player, int deltaTime)
{
    this->map = &tilemap;
    this->targetPlayer = &player;

    timeSinceLastShot += deltaTime;

    bool canSee = canSeePlayer(player, tilemap);

    switch (state)
    {
    case GUARD_IDLE:
        idleTimer += deltaTime;
        showZZZ = true;

        if (canSee)
        {
            state = GUARD_ALERT;
            playerDetected = true;
            showZZZ = false;
            showAlert = true;
            alertTimer = 0.0f;
        }
        else if (idleTimer >= directionChangeTimer)
        {
            state = GUARD_PATROL;
            idleTimer = 0;
            showZZZ = false;
            chooseRandomDirection();
            patrolTimer = 0;
            directionChangeTimer = rand() % (PATROL_TIME_MAX - PATROL_TIME_MIN) + PATROL_TIME_MIN;
        }
        break;

    case GUARD_PATROL:
        updatePatrol(deltaTime, tilemap);
        showZZZ = false;

        if (canSee)
        {
            state = GUARD_ALERT;
            currentDirection = NONE;
            playerDetected = true;
            showAlert = true;
            alertTimer = 0.0f;
        }
        break;

    case GUARD_ALERT:
        updateAlert(deltaTime, player);
        showZZZ = false;

        if (canSee)
        {
            state = GUARD_SHOOTING;
        }
        else
        {
            state = GUARD_IDLE;
            playerDetected = false;
            showAlert = false;
            showZZZ = true;
            idleTimer = 0;
            directionChangeTimer = rand() % (IDLE_TIME_MAX - IDLE_TIME_MIN) + IDLE_TIME_MIN;
        }
        break;

    case GUARD_SHOOTING:
        updateShooting(deltaTime, player);
        showZZZ = false;

        if (!canSee)
        {
            state = GUARD_IDLE;
            playerDetected = false;
            showAlert = false;
            showZZZ = true;
        }
        break;
    }
}

void Guard::updatePatrol(int deltaTime, TileMap& tilemap)
{
    patrolTimer += deltaTime;

    if (currentDirection != NONE)
    {
        moveInDirection(deltaTime, tilemap);

        int anim = sprite->animation();
        switch (currentDirection)
        {
        case LEFT:
            if (anim != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
            break;
        case RIGHT:
            if (anim != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
            break;
        case UP:
            if (anim != MOVE_UP) sprite->changeAnimation(MOVE_UP);
            break;
        case DOWN:
            if (anim != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
            break;
        default:
            break;
        }
    }


    if (patrolTimer >= directionChangeTimer)
    {
        patrolTimer = 0;
        chooseRandomDirection();
        directionChangeTimer = rand() % (PATROL_TIME_MAX - PATROL_TIME_MIN) + PATROL_TIME_MIN;
    }
}


void Guard::updateAlert(int deltaTime, Player& player)
{
    glm::vec2 playerPos = player.getPosition();
    glm::vec2 dirToPlayer = playerPos - glm::vec2(posGuard);

    facingDirection = (dirToPlayer.x > 0) ? RIGHT : LEFT;

    if (facingDirection == LEFT)
        sprite->changeAnimation(STAND_LEFT);
    else
        sprite->changeAnimation(STAND_RIGHT);

}

void Guard::updateShooting(int deltaTime, Player& player)
{
    glm::vec2 playerPos = player.getPosition();
    glm::vec2 dirToPlayer = playerPos - glm::vec2(posGuard);

    facingDirection = (dirToPlayer.x > 0) ? RIGHT : LEFT;

    if (timeSinceLastShot >= fireCooldown)
    {
        shootAtPlayer(player);
    }

    if (facingDirection == LEFT)
        sprite->changeAnimation(STAND_LEFT);
    else
        sprite->changeAnimation(STAND_RIGHT);
}

void Guard::shootAtPlayer(Player& player)
{
    if (player.getIsDead()) return;
    if (timeSinceLastShot < fireCooldown) return;

    if (alive)
    {
        timeSinceLastShot = 0;

        showAlert = true;
        alertTimer = 0.0f;
        showZZZ = false;

        glm::vec2 playerPos = player.getPosition();
        glm::vec2 direction = glm::normalize(playerPos - glm::vec2(posGuard));

        Bullet* bullet = new Bullet(glm::vec2(posGuard) + glm::vec2(16, 16), direction, shaderProgram, BulletType::GUARD);
        bullet->setAlive(true);
        bullets.push_back(bullet);

        if (direction.x < 0)
            sprite->changeAnimation(STAND_LEFT);
        else
            sprite->changeAnimation(STAND_RIGHT);
    }
}

bool Guard::canSeePlayer(Player& player, TileMap& tilemap)
{
    if (player.getIsDead()) return false;

    glm::vec2 playerPos = player.getPosition();
    glm::vec2 guardPos = glm::vec2(posGuard);

    float distance = glm::length(playerPos - guardPos);

    if (distance > detectionRange)
    {
        return false;
    }

    bool canSee = isPlayerInLineOfSight(playerPos, tilemap);

    return canSee;
}

bool Guard::isPlayerInLineOfSight(const glm::vec2& playerPos, TileMap& tilemap)
{
    glm::vec2 guardPos = glm::vec2(posGuard) + glm::vec2(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
    glm::vec2 playerCenter = playerPos + glm::vec2(16, 26);
    glm::vec2 diff = playerCenter - guardPos;

    int tileSize = tilemap.getTileSize();
    float tolerance = tileSize * 2.0f;

    bool sameRow = abs(diff.y) < tolerance;
    bool sameCol = abs(diff.x) < tolerance;

    if (!sameRow && !sameCol)
    {
        return false;
    }

    glm::vec2 direction = glm::normalize(diff);
    float distance = glm::length(diff);

    for (float d = tileSize; d < distance; d += tileSize / 2.0f)
    {
        glm::vec2 checkPos = guardPos + direction * d;
        glm::ivec2 tile = tilemap.worldToTileCoords(checkPos);

        if (!tilemap.isWalkable(tile.x, tile.y))
        {
            return false;
        }
    }

    return true;
}

void Guard::moveInDirection(int deltaTime, TileMap& tilemap)
{
    if (currentDirection == NONE)
    {
        return;
    }

    glm::vec2 newPos = posGuard;
    float delta = moveSpeed * (deltaTime / 1000.0f);

    switch (currentDirection)
    {
    case LEFT:
        newPos.x -= delta;
        break;
    case RIGHT:
        newPos.x += delta;
        break;
    case UP:
        newPos.y -= delta;
        break;
    case DOWN:
        newPos.y += delta;
        break;
    }

    if (canMove(newPos, tilemap))
    {
        posGuard = newPos;
        sprite->setPosition(posGuard);
        facingDirection = currentDirection;

        static int debugCounter = 0;
        debugCounter += deltaTime;
        if (debugCounter > 500) {
            debugCounter = 0;
        }
    }
    else
    {

        switch (currentDirection)
        {
        case LEFT:  currentDirection = RIGHT; break;
        case RIGHT: currentDirection = LEFT; break;
        case UP:    currentDirection = DOWN; break;
        case DOWN:  currentDirection = UP; break;
        default:    break;
        }

        facingDirection = currentDirection;

        switch (currentDirection)
        {
        case LEFT:  sprite->changeAnimation(MOVE_LEFT);  break;
        case RIGHT: sprite->changeAnimation(MOVE_RIGHT); break;
        case UP:    sprite->changeAnimation(MOVE_UP);    break;
        case DOWN:  sprite->changeAnimation(MOVE_DOWN);  break;
        }
    }

}

bool Guard::canMove(const glm::vec2& newPos, TileMap& tilemap)
{
    int tileSize = tilemap.getTileSize();

    glm::vec2 topLeft = newPos + glm::vec2(2, 2);
    glm::vec2 topRight = newPos + glm::vec2(SPRITE_WIDTH - 2, 2);
    glm::vec2 bottomLeft = newPos + glm::vec2(2, SPRITE_HEIGHT - 2);
    glm::vec2 bottomRight = newPos + glm::vec2(SPRITE_WIDTH - 2, SPRITE_HEIGHT - 2);

    auto check = [&](glm::vec2 p) {
        glm::ivec2 tile = tilemap.worldToTileCoords(p);
        return tilemap.isWalkable(tile.x, tile.y);
        };

    return check(topLeft) && check(topRight) && check(bottomLeft) && check(bottomRight);
}


void Guard::chooseRandomDirection()
{
    int choice = rand() % 4;
    switch (choice)
    {
    case 0: currentDirection = LEFT; break;
    case 1: currentDirection = RIGHT; break;
    case 2: currentDirection = UP; break;
    case 3: currentDirection = DOWN; break;
    }

    switch (currentDirection)
    {
    case LEFT:  sprite->changeAnimation(MOVE_LEFT);  break;
    case RIGHT: sprite->changeAnimation(MOVE_RIGHT); break;
    case UP:    sprite->changeAnimation(MOVE_UP);    break;
    case DOWN:  sprite->changeAnimation(MOVE_DOWN);  break;
    }

    facingDirection = currentDirection;
}


bool Guard::moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime)
{
    const float speedPixelsPerSecond = 60.0f;
    const float threshold = 2.0f;

    glm::vec2 targetWorld = tilemap.tileToWorldCoords(nextTile);
    float offsetX = (tilemap.getTileSize() - 32) * 0.5f;
    float offsetY = (tilemap.getTileSize() - 62) * 0.5f;
    targetWorld += glm::vec2(offsetX, offsetY);

    glm::vec2 dir = targetWorld - glm::vec2(posGuard);
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < threshold)
    {
        posGuard = glm::ivec2(targetWorld);
        sprite->setPosition(glm::vec2(posGuard));
        return true;
    }

    float step = speedPixelsPerSecond * (deltaTime / 1000.0f);
    glm::vec2 movement = (dist <= step) ? dir : (dir / dist) * step;

    if (abs(movement.x) > abs(movement.y))
    {
        if (movement.x > 0)
            sprite->changeAnimation(MOVE_RIGHT);
        else
            sprite->changeAnimation(MOVE_LEFT);
    }

    posGuard += glm::ivec2(movement);
    sprite->setPosition(glm::vec2(posGuard));

    return false;
}

void Guard::update(int deltaTime)
{
    if (alive)
    {
        sprite->update(deltaTime);

        static float t = 0.f;
        t += deltaTime / 1000.f;

        if (spriteAlert && showAlert)
        {
            alertTimer += deltaTime;
            float bounce = sin(t * 10.f) * 3.f;
            spriteAlert->setPosition(glm::vec2(posGuard.x + 8, posGuard.y - 32 + bounce));

            if (alertTimer >= ALERT_DURATION)
                showAlert = false;
        }

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
}


void Guard::render()
{
    if (alive) {
        sprite->render();

        if (showAlert && spriteAlert)
            spriteAlert->render();

        for (Bullet* b : bullets)
            b->render();
    }
}


void Guard::setPosition(const glm::vec2& pos)
{
    posGuard = pos; 
    sprite->setPosition(pos);
}

void Guard::setShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
}

void Guard::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;

    if (health <= 0)
    {
        alive = false;
    }
}

void Guard::clearBullets()
{
    for (Bullet* b : bullets)
        delete b;
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
    timeSinceLastShot = fireCooldown;
    sprite->changeAnimation(STAND_RIGHT);

    state = GUARD_IDLE;
    currentDirection = NONE;
    facingDirection = RIGHT;
    showZZZ = true;
    showAlert = false;
    idleTimer = 0;
    patrolTimer = 0;
    directionChangeTimer = rand() % (IDLE_TIME_MAX - IDLE_TIME_MIN) + IDLE_TIME_MIN;
}