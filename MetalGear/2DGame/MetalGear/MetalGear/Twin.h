#ifndef _TWIN_INCLUDE
#define _TWIN_INCLUDE
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "Bullet.h"
#include <list>

#define SPRITESHEET_OFFSET 0.16666666666666666666666666666667
#define SPRITE_WIDTH 16*2 
#define SPRITE_HEIGHT 31*2 

class Twin
{
public:
    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setPosition(const glm::vec2& pos);
    void AIControl(TileMap& tilemap, Player& player, int deltaTime);
    void updateVerticalMovement(int deltaTime, TileMap& tilemap);
    void shootAtPlayer(Player& player);
    void setShaderProgram(ShaderProgram* program);
    void takeDamage(int dmg);
    bool isDead() const { return health <= 0; }
    const std::list<Bullet*>& getBullets() const { return bullets; }
    void clearBullets();
    void reset();

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 posTwin;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map = nullptr;

    // Variables de movimiento vertical
    bool movingDown = true;
    float verticalSpeed = 120.0f;

    // ? Variables para alternar animaciones
    int animationTimer = 0;
    int animationSwitchTime = 300; // ms entre cambios de animación

    // Variables de disparo
    int fireCooldown = 200;
    int timeSinceLastShot = 0;
    std::list<Bullet*> bullets;

    ShaderProgram* shaderProgram;
    int health = 50;
    bool alive = true;

    Player* targetPlayer = nullptr;
};

#endif // _TWIN_INCLUDE