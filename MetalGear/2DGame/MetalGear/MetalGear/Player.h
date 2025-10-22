#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Level.h"

#define SPRITESHEET_OFFSET 0.16666666666666666666666666666667
#define SPRITE_WIDTH 16*2
#define SPRITE_HEIGHT 31*2

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	//void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void init(ShaderProgram& shaderProgram);
	bool WASDMovementControl();
	void update(int deltaTime);
	void render();

	bool changeMap_tile(int &tileType, char &dir);
	glm::ivec2 getPosition();
	void lookLeft();
	void lookRight();
	void lookDown();
	void lookUp();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	void setPause() { pause = true; }
	void setStopPause() { pause = false; }
	void setDirection(const char& dir) { direction = dir; }
	void setLevel(Level *level);

	void takeDamage(int dmg);
	bool isDead() const { return health <= 0; }
	int getHealth() const { return health; }
	void setHealth(int h);
	void reset();


private:
	glm::ivec2 tileMapDispl, posPlayer;
	int startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	glm::bvec4 movementControl;
	int health = 100;

	char direction;
	std::vector<Weapon*> weapons;
	std::vector<AccessCard*> accessCards;
	std::vector<Meal*> meals;
	Weapon* punch;

	Level* level;

	bool pause;
};


#endif // _PLAYER_INCLUDE


