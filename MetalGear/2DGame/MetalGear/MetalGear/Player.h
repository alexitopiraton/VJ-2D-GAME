#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Level.h"
#include "Bullet.h"
#include "Gui.h"
#include <list>

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

	bool getChangeMap() const { return changeMap; }
	int getMapToChange() const { return mapToChange; }
	void setChangeMap() { changeMap = false; }

	void resetCollectAllItems() { collectAllItems = false; }
	bool hasObject(Object* object) const;
	void setActiveObject();
	void addObject(Object* obj);
	void clearAllObjects();
	int getObjectCount() const;
	std::vector<string> getActiveObjectName() const;
	bool getCollectAllItems() const { return collectAllItems; }
	bool getGodMode() const { return godMode; }

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	void setPause() { pause = true; }
	void setStopPause() { pause = false; }
	void setDirection(const char& dir) { direction = dir; }
	void setLevel(Level *level);
	void setGui(Gui* g);
	void setLevelToGUI();
	void setHealth(const int& value) { health = value; }

	bool isDead() const { return health <= 0; }
	void reset();
	void takeDamage(int dmg);

	void shoot();
	glm::vec2 directionConversor();
	list<Bullet*> getBullets() { return bullets; }
	int getWeaponDamage();
	void clearBullets();

private:
	glm::ivec2 tileMapDispl, posPlayer;
	int startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Gui* gui;

	glm::bvec4 movementControl;

	char direction;
	std::vector<Object*> objects;
	int activeObject;
	float cooldownKey;
	Weapon* punch;

	Level* level;
	bool pause, erased, collectAllItems, godMode;
	int health;

	bool changeMap;
	int mapToChange;

	list<Bullet*> bullets;
	int fireCooldown = 200;
	int timeSinceLastShot = 0;

	ShaderProgram program;
};


#endif // _PLAYER_INCLUDE


