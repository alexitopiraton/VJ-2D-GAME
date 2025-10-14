#ifndef _GUARD_INCLUDE
#define _GUARD_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

#include "Bullet.h"
#include <list>

#define SPRITESHEET_OFFSET 0.16666666666666666666666666666667
#define SPRITE_WIDTH 16*2
#define SPRITE_HEIGHT	31*2



// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Guard
{

public:
	//void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	//void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void AIControl(TileMap& tilemap, Player& player, int deltaTime);
	// deltaTime en ms (puedes adaptarlo si no usas deltaTime)
	bool moveTowardsTile(const glm::ivec2& nextTile, TileMap& tilemap, int deltaTime);
	void shootAtPlayer(Player& player);
	void setShaderProgram(ShaderProgram* program);

	void takeDamage(int dmg);
	bool isDead() const { return health <= 0; }
	const std::list<Bullet*>& getBullets() const { return bullets; }
	void clearBullets();
	void reset();


private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posGuard;
	int startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	glm::bvec4 movementControl;

	std::vector<glm::ivec2> currentPath; // path calculado hacia el jugador
	int currentPathIndex = 0;            // índice del tile actual del path
	glm::ivec2 lastPlayerTile = { -1, -1 }; // último tile del jugador para saber si recalcular
	int timeSinceLastPath = 0;
	const int pathRecalcCooldown = 200;

	float detectionRadius = 5.0f;  // en tiles — distancia máxima a la que detecta al jugador
	float attackRadius = 4.0f;     // en tiles — si estás dentro, dispara
	int fireCooldown = 1000;       // ms entre disparos
	int timeSinceLastShot = 0;
	std::list<Bullet*> bullets;
	ShaderProgram* shaderProgram;

	int health = 50;
	bool alive = true;
};


#endif // _PLAYER_INCLUDE
#pragma once
