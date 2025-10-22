#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "TileMap.h"
#include "Weapon.h"
#include "Meal.h"
#include "AccessCard.h"
#include <vector>
#include <utility>
#include <iostream>
#include "Player.h"
#include "Guard.h"
#include "Roller.h"
#include "ArnoldBoss.h"
#include "Twin.h"

#define IMAGE_OFFSET 0.33333333333333333333333333333333

class Level
{
public:
	Level();
	~Level();

	TileMap* get_tile_map();
	void init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet, const std::vector<string>& objectTypes, const std::vector<std::pair<int, int>>& objectPositions);
	void render();
	void spriteToHide(const string& hide, const glm::vec2 &tileCoords, const int &tile);

	void setPause() { pause = true; }
	void setStopPause() { pause = false; }
	void setBlackScreen();

	Weapon* getWeapon() { return weapon; }
	AccessCard* getAccessCard() { return accessCard; }
	Meal* getMeal() { return meal; }
  
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

	void addTwin(const glm::vec2& position, ShaderProgram& program);
	void resetTwin();


private:
	TileMap* map;
	Texture backgroundImage;
	Texture blackScreenImage;
	Sprite* background;
	Sprite* blackScreen;

	Weapon* weapon;
	Meal* meal;
	AccessCard* accessCard;

	bool pause;
	bool hideWeapon;
	bool hideAccessCard;
	bool hideMeal;
	std::vector<Object> objects;
	std::vector<Guard*> guards;
	std::vector<Roller*> rollers;
	ArnoldBoss* arnoldBoss = nullptr;
	Twin* twin = nullptr;
};

#endif