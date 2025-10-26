#ifndef _ARNOLD_BOSS_INCLUDE
#define _ARNOLD_BOSS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Pathfinder.h"

class Player;

class ArnoldBoss
{
public:
    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime, TileMap& tilemap, Player& player);
    void render();
    void setPosition(const glm::vec2& pos);
    void takeDamage(int dmg);
    bool isAlive() const { return health > 0; }
    void setShaderProgram(ShaderProgram* program);
    glm::ivec2 findClosestWalkableTile(TileMap& tilemap, const glm::ivec2& tile);

    glm::vec2 getPosition() { return posBoss; }
    void reset();

private:
    bool moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime);

private:
    Texture spritesheet;
    Sprite* sprite;
    glm::vec2 initialPosBoss;
    glm::vec2 posBoss;
    std::vector<glm::ivec2> currentPath;
    int currentPathIndex = 0;
    glm::ivec2 lastPlayerTile = { -1, -1 };
    int timeSinceLastPath = 0;
    const int pathRecalcCooldown = 100; // recalcula más rápido que los guardias
    int health = 300; // mucha vida
    float speed = 120.f; // más lento pero constante
    ShaderProgram* shaderProgram;
    bool isDead;
};
#endif