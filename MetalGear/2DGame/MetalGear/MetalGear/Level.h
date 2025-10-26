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

class Guard;
class Player;
class Roller;
class ArnoldBoss;
class Twin;
class Player;

class Level
{
public:
	Level();
	~Level();

	TileMap* get_tile_map();
	void init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet, const std::vector<string>& objectTypes, const std::vector<std::pair<int, int>>& objectPositions, const int &num, const std::vector<int>& accessCardNumbers, int &iterator);
	void render();
	void spriteToHide(const string& hide, const glm::vec2 &tileCoords, const int &tile);

	void setPause() { pause = true; }
	void setStopPause() { pause = false; }
	void setBlackScreen();
	void setDoorOpen(const bool& open);

	int getId() { return id; }
	Weapon* getWeapon() { return weapon; }
	AccessCard* getAccessCard() { return accessCard; }
	Meal* getMeal() { return meal; }

	bool isDoorOpen() { return map->isDoorOpen(); }

	bool hasWeapon() const;
	bool hasAccessCard() const;
	bool hasMeal() const;
	void collectAllObjects();
	void hideObjectWeapon();
	void hideObjectAccessCard();
	void hideObjectMeal();
	void regenerateWeapon(ShaderProgram& program);
	void regenerateAccessCard(ShaderProgram& program);
	void regenerateMeal(ShaderProgram& program);

	void addGuard(const glm::vec2& position, ShaderProgram& program);
	void update(int deltaTime, Player* player);
	const vector<Guard*>& getGuards() const { return guards; }
	void resetEnemies();

	void addRoller(const glm::vec2& pos, ShaderProgram& shaderProgram, bool moveRight);
	void updateRollers(int deltaTime, Player* player);
	void renderRollers();
	void clearRollers();

	void addArnoldBoss(const glm::vec2& pos, ShaderProgram& shaderProgram);

	void addTwin(const glm::vec2& position, ShaderProgram& program);
	void resetTwin();

	void reset();

private:
	int id;

	TileMap* map;
	Texture backgroundImage;
	Texture blackScreenImage;
	Texture openedVersionImage;
	Sprite* background;
	Sprite* blackScreen;
	Sprite* openedVersion;

	Weapon* weapon;
	Meal* meal;
	AccessCard* accessCard;

	bool pause;
	bool hideWeapon;
	bool hideAccessCard;
	bool hideMeal;
	int iterator;
	int numEnemies;
	bool doorOpened;

	glm::ivec2 weaponOriginalPos;
	glm::ivec2 accessCardOriginalPos;
	glm::ivec2 mealOriginalPos;
	int accessCardNum;
	int accessCardId;
	bool hasWeaponOriginal;
	bool hasAccessCardOriginal;
	bool hasMealOriginal;

	std::vector<Guard*> guards;
	std::vector<Roller*> rollers;
	ArnoldBoss* arnoldBoss = nullptr;
	Twin* twin = nullptr;
};

#endif