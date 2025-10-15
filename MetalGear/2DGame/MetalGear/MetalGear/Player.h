#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

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

	bool changeMap_tile(int &tileType, char &direction);
	glm::ivec2 getPosition();
	void lookLeft();
	void lookRight();
	void lookDown();
	void lookUp();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	void setPause() { pause = true; }
	void setStopPause() { pause = false; }

private:
	glm::ivec2 tileMapDispl, posPlayer;
	int startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	glm::bvec4 movementControl;

	bool pause;
};


#endif // _PLAYER_INCLUDE


