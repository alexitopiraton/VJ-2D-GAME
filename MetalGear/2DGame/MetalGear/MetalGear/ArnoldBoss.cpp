#include "ArnoldBoss.h"
#include "Game.h"
#include <iostream>

enum ArnoldAnim {
    MOVE_LEFT, MOVE_RIGHT
};

void ArnoldBoss::init(ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("images/enemies/arnold.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0f, 0.5f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(MOVE_LEFT, 8);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 8);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

    sprite->changeAnimation(MOVE_RIGHT);
    sprite->setPosition(glm::vec2(posBoss.x, posBoss.y));

	cout << "[ArnoldBoss] ¡Arnold inicializado!" << endl;
}

void ArnoldBoss::update(int deltaTime, TileMap& tilemap, Player& player)
{
    /*if (health <= 0) return;

    timeSinceLastPath += deltaTime;

    glm::ivec2 bossTile = tilemap.worldToTileCoords(posBoss + glm::vec2(32, 32));
    glm::ivec2 playerTile = tilemap.worldToTileCoords(glm::vec2(player.getPosition()) + glm::vec2(16, 16));

    if (timeSinceLastPath >= pathRecalcCooldown) {
        timeSinceLastPath = 0;

        if (playerTile != lastPlayerTile) {
            lastPlayerTile = playerTile;

            Pathfinder pf(&tilemap);
            if (pf.findPath(bossTile, playerTile, currentPath)) {
                currentPathIndex = 1;
            }
            else {
                currentPath.clear();
                currentPathIndex = 0;
            }
        }
    }

    if (!currentPath.empty() && currentPathIndex < currentPath.size()) {
        bool reached = moveTowardsTile(currentPath[currentPathIndex], tilemap, deltaTime);
        if (reached) currentPathIndex++;
    }

    // Colisión con jugador ? muerte instantánea
    float dist = glm::length(glm::vec2(posBoss) - glm::vec2(player.getPosition()));
    if (dist < 32.f) {
        player.takeDamage(100); // muerte instantánea
    }*/

    sprite->update(deltaTime);
	cout << "[ArnoldBoss] Update llamado, pero IA desactivada temporalmente." << endl;
}

bool ArnoldBoss::moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime)
{
    glm::vec2 target = tilemap.tileToWorldCoords(nextTile);
    glm::vec2 dir = target - posBoss;
    float dist = glm::length(dir);

    if (dist < 1.0f) {
        posBoss = target;
        sprite->setPosition(posBoss);
        return true;
    }

    glm::vec2 movement = (dir / dist) * (speed * (deltaTime / 1000.f));
    posBoss += movement;
    sprite->setPosition(posBoss);

    if (movement.x > 0)
        sprite->changeAnimation(MOVE_RIGHT);
    else
        sprite->changeAnimation(MOVE_LEFT);

    return false;
}

void ArnoldBoss::render()
{
    sprite->render();
}

void ArnoldBoss::setPosition(const glm::vec2& pos)
{
    posBoss = glm::ivec2(pos);
    sprite->setPosition(glm::vec2(float(posBoss.x), float(posBoss.y)));
}

void ArnoldBoss::setShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
}

void ArnoldBoss::takeDamage(int dmg)
{
    health -= dmg;
    if (health <= 0) {
        std::cout << "[ArnoldBoss] ¡Derrotado!" << std::endl;
    }
    else {
        std::cout << "[ArnoldBoss] Vida restante: " << health << std::endl;
    }
}
