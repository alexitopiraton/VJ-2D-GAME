#ifndef _GUARD_INCLUDE
#define _GUARD_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Bullet.h"
#include <list>
#include <vector>

class Player;

#define SPRITESHEET_OFFSET 0.16666666666666666666666666666667
#define SPRITE_WIDTH 16*2 
#define SPRITE_HEIGHT 31*2 

enum GuardState {
    GUARD_IDLE,      
    GUARD_PATROL,    
    GUARD_ALERT,      
    GUARD_SHOOTING   
};

class Guard
{
public:
    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setPosition(const glm::vec2& pos);
    void AIControl(TileMap& tilemap, Player& player, int deltaTime);
    bool moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime);
    void shootAtPlayer(Player& player);
    void setShaderProgram(ShaderProgram* program);
    void takeDamage(int dmg);
    bool isDead() const { return health <= 0; }
    const std::list<Bullet*>& getBullets() const { return bullets; }
    void clearBullets();
    void reset();

    glm::vec2 getPosition() { return posGuard; }
    bool isAlive() const { return alive; }

private:

    bool canSeePlayer(Player& player, TileMap& tilemap);
    bool isPlayerInLineOfSight(const glm::vec2& playerPos, TileMap& tilemap);

    
    void updatePatrol(int deltaTime, TileMap& tilemap);
    void updateAlert(int deltaTime, Player& player);
    void updateShooting(int deltaTime, Player& player);

    
    void moveInDirection(int deltaTime, TileMap& tilemap);
    void chooseRandomDirection();
    bool canMove(const glm::vec2& newPos, TileMap& tilemap);

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 posGuard;
    int startY;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    glm::bvec4 movementControl;

    
    std::vector<glm::ivec2> currentPath;
    int currentPathIndex = 0;
    glm::ivec2 lastPlayerTile = { -1, -1 };
    int timeSinceLastPath = 0;
    const int pathRecalcCooldown = 200;

    
    float detectionRadius = 5.0f;  
    float attackRadius = 4.0f;     
    int fireCooldown = 1000;       
    int timeSinceLastShot = 0;

    std::list<Bullet*> bullets;
    ShaderProgram* shaderProgram;

    int health = 50;
    bool alive = true;

    
    Sprite* spriteZZZ = nullptr;
    bool showZZZ = false;
    Texture spritesheetZZZ;

    Texture alertTexture;
    Sprite* spriteAlert = nullptr;
    bool showAlert = false;
    float alertTimer = 0.0f;
    const float ALERT_DURATION = 1000.0f;

    
    GuardState state = GUARD_IDLE;

    enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
    Direction currentDirection = NONE;
    Direction facingDirection = RIGHT;

    float moveSpeed = 30.0f;       
    float patrolSpeed = 30.0f;
    float detectionRange = 400.0f;  

    int idleTimer = 0;
    int patrolTimer = 0;
    int directionChangeTimer = 2000;

    bool playerDetected = false;
    Player* targetPlayer = nullptr;
};

#endif // _PLAYER_INCLUDE