#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "TileMap.h"
#include "Object.h"
#include <vector>

#define IMAGE_OFFSET 0.33333333333333333333333333333333

class Level
{
public:
	Level();
	~Level();

	TileMap* get_tile_map();
	void init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet);
	void render();

private:
	TileMap* map;
	Texture backgroundImage;
	Sprite* background;
	std::vector<Object> objects;
};

#endif