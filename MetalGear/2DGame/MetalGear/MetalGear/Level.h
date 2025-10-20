#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "TileMap.h"
#include "Weapon.h"
#include "Meal.h"
#include "AccessCard.h"
#include <vector>
#include <utility>
#include <iostream>

#define IMAGE_OFFSET 0.33333333333333333333333333333333

class Level
{
public:
	Level();
	~Level();

	TileMap* get_tile_map();
	void init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet, const std::vector<string>& objectTypes, const std::vector<std::pair<int, int>>& objectPositions);
	void render();
	void spriteToHide(const string& hide);

	void setPause() { pause = true; }
	void setStopPause() { pause = false; }
	void setBlackScreen();

	Weapon* getWeapon() { return weapon; }
	AccessCard* getAccessCard() { return accessCard; }
	Meal* getMeal() { return meal; }

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
};

#endif