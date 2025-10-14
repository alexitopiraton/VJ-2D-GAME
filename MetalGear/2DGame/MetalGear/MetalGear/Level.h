#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "TileMap.h"
#include "Object.h"
#include <vector>
#include "Player.h"
#include "Guard.h"
#include "Roller.h"
#include "ArnoldBoss.h"

#define IMAGE_OFFSET 0.33333333333333333333333333333333

class Level
{
public:
	Level();
	~Level();

	TileMap* get_tile_map();
	void init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet);

	void addGuard(const glm::vec2& position, ShaderProgram& program);
	void render();
	void update(int deltaTime, Player* player);
	const vector<Guard*>& getGuards() const { return guards; }
	void resetGuards();

	void addRoller(const glm::vec2& pos, ShaderProgram& shaderProgram, bool moveRight);
	void updateRollers(int deltaTime, Player* player);
	void renderRollers();
	void clearRollers();

	void addArnoldBoss(const glm::vec2& pos, ShaderProgram& shaderProgram);


private:
	TileMap* map;
	Texture backgroundImage;
	Sprite* background;
	std::vector<Object> objects;
	std::vector<Guard*> guards;
	std::vector<Roller*> rollers;
	ArnoldBoss* arnoldBoss = nullptr;
};

#endif