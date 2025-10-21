#include "ArnoldBoss.h"
#include "Game.h"
#include <iostream>

enum ArnoldAnim {
    MOVE_LEFT, MOVE_RIGHT
};

void ArnoldBoss::init(ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("images/enemies/arnold.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.5f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(MOVE_LEFT, 6);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 6);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

    sprite->changeAnimation(MOVE_RIGHT);
    sprite->setPosition(glm::vec2(posBoss.x, posBoss.y));

	cout << "[ArnoldBoss] ¡Arnold inicializado!" << endl;
}
void ArnoldBoss::update(int deltaTime, TileMap& tilemap, Player& player)
{
    if (health <= 0) return;

    timeSinceLastPath += deltaTime;

    // -------------------
    // 1?? Obtener posición del jugador (en píxeles)
    // -------------------
    glm::vec2 playerPos = glm::vec2(player.getPosition());
    float distToPlayer = glm::length(playerPos - posBoss);

    // -------------------
    // 2?? Si está muy cerca del jugador, ir directo (sin pathfinding)
    // -------------------
    if (distToPlayer < tilemap.getTileSize() * 3) { // Menos de 3 tiles
        // Movimiento directo hacia el jugador
        glm::vec2 dir = playerPos - posBoss;
        float dist = glm::length(dir);

        if (dist > 1.0f) {
            dir = glm::normalize(dir);
            float step = speed * (deltaTime / 1000.0f);

            glm::vec2 newPos = posBoss + dir * step;

            // Verificar que la nueva posición sea walkable
            glm::ivec2 newTile = tilemap.worldToTileCoords(newPos + glm::vec2(32, 32)); // centro del sprite
            if (tilemap.isWalkable(newTile.x, newTile.y)) {
                posBoss = newPos;
                sprite->setPosition(posBoss);

                // Actualizar animación según dirección
                if (dir.x > 0.1f) {
                    if (sprite->animation() != MOVE_RIGHT) {
                        sprite->changeAnimation(MOVE_RIGHT);
                    }
                }
                else if (dir.x < -0.1f) {
                    if (sprite->animation() != MOVE_LEFT) {
                        sprite->changeAnimation(MOVE_LEFT);
                    }
                }
            }
        }

        // IMPORTANTE: Actualizar sprite DESPUÉS de mover
        sprite->update(deltaTime);

        // Comprobar colisión
        if (distToPlayer < 32.f) {
            player.takeDamage(100);
        }
        return; // Salir sin usar pathfinding
    }

    // -------------------
    // 3?? Obtener tiles del boss y jugador
    // -------------------
    glm::ivec2 bossTile = tilemap.worldToTileCoords(posBoss + glm::vec2(32, 32));
    glm::ivec2 playerTile = tilemap.worldToTileCoords(playerPos + glm::vec2(16, 16));

    // Ajustar si alguno no es walkable
    if (!tilemap.isWalkable(bossTile.x, bossTile.y)) {
        bossTile = findClosestWalkableTile(tilemap, bossTile);
    }
    if (!tilemap.isWalkable(playerTile.x, playerTile.y)) {
        playerTile = findClosestWalkableTile(tilemap, playerTile);
    }

    // -------------------
    // 4?? Recalcular path cuando sea necesario
    // -------------------
    const int PATH_RECALC_INTERVAL = 400; // ms

    bool playerMovedTile = (playerTile != lastPlayerTile);
    bool timeToRecalc = (timeSinceLastPath >= PATH_RECALC_INTERVAL);
    bool noValidPath = (currentPath.empty() || currentPathIndex >= currentPath.size());

    if (playerMovedTile || (timeToRecalc && noValidPath)) {
        lastPlayerTile = playerTile;
        timeSinceLastPath = 0;

        Pathfinder pf(&tilemap);
        std::vector<glm::ivec2> newPath;

        if (pf.findPath(bossTile, playerTile, newPath)) {
            currentPath = newPath;
            currentPathIndex = 0;

            // Si ya estamos en el primer tile del path, avanzar al siguiente
            if (!currentPath.empty() && currentPath[0] == bossTile && currentPath.size() > 1) {
                currentPathIndex = 1;
            }

            std::cout << "[Boss] Path encontrado: " << currentPath.size() << " tiles" << std::endl;
        }
        else {
            std::cout << "[Boss] No hay camino disponible" << std::endl;
            currentPath.clear();
            currentPathIndex = 0;
        }
    }

    // -------------------
    // 5?? Seguir el path
    // -------------------
    if (!currentPath.empty() && currentPathIndex < currentPath.size()) {
        glm::ivec2 targetTile = currentPath[currentPathIndex];

        // Verificar que el tile objetivo sigue siendo válido
        if (!tilemap.isWalkable(targetTile.x, targetTile.y)) {
            std::cout << "[Boss] Tile bloqueado, recalculando..." << std::endl;
            currentPath.clear();
            currentPathIndex = 0;
            timeSinceLastPath = PATH_RECALC_INTERVAL;
        }
        else {
            bool reached = moveTowardsTile(targetTile, tilemap, deltaTime);
            if (reached) {
                currentPathIndex++;
            }
        }
    }

    // -------------------
    // 6?? Comprobar colisión con jugador
    // -------------------
    if (distToPlayer < 32.f) {
        player.takeDamage(100);
    }

    // -------------------
    // 7?? Actualizar sprite
    // -------------------
    sprite->update(deltaTime);
}


bool ArnoldBoss::moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime)
{
    const int tileSize = tilemap.getTileSize();
    const glm::ivec2 spriteSize(64, 64);
    const float speedPixelsPerSecond = speed;

    glm::vec2 targetWorld = tilemap.tileToWorldCoords(nextTile);
    targetWorld.x += (tileSize - spriteSize.x) * 0.5f;
    targetWorld.y += (tileSize - spriteSize.y) * 0.5f;

    glm::vec2 dir = targetWorld - posBoss;
    float dist = glm::length(dir);
    const float arriveThreshold = 3.0f;

    if (dist < arriveThreshold) {
        posBoss = targetWorld;
        sprite->setPosition(posBoss);
        return true;
    }

    dir = glm::normalize(dir);
    float step = speedPixelsPerSecond * (deltaTime / 1000.0f);

    if (dist <= step)
        posBoss = targetWorld;
    else
        posBoss += dir * step;

    // --- Animación continua ---
    if (dir.x > 0.1f) {
        if (sprite->animation() != MOVE_RIGHT)
            sprite->changeAnimation(MOVE_RIGHT);
    }
    else if (dir.x < -0.1f) {
        if (sprite->animation() != MOVE_LEFT)
            sprite->changeAnimation(MOVE_LEFT);
    }

    sprite->setPosition(posBoss);

    // ? ACTUALIZA la animación siempre que se mueva
    sprite->update(deltaTime);

    return false;
}



glm::ivec2 ArnoldBoss::findClosestWalkableTile(TileMap& tilemap, const glm::ivec2& tile)
{
    if (tilemap.isWalkable(tile.x, tile.y))
        return tile;

    // Búsqueda en espiral
    const int maxRadius = 5;
    for (int radius = 1; radius <= maxRadius; radius++) {
        // Revisar tiles alrededor en este radio
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dy = -radius; dy <= radius; dy++) {
                // Solo revisar el borde del cuadrado actual
                if (abs(dx) != radius && abs(dy) != radius)
                    continue;

                int nx = tile.x + dx;
                int ny = tile.y + dy;

                if (nx < 0 || ny < 0 || nx >= tilemap.getWidth() || ny >= tilemap.getHeight())
                    continue;

                if (tilemap.isWalkable(nx, ny)) {
                    std::cout << "[Boss] Tile alternativo encontrado: (" << nx << ", " << ny << ")" << std::endl;
                    return glm::ivec2(nx, ny);
                }
            }
        }
    }

    // Si no se encuentra nada, devolver el tile original
    std::cout << "[Boss] ADVERTENCIA: No se encontró tile walkable cerca de ("
        << tile.x << ", " << tile.y << ")" << std::endl;
    return tile;
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
