#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


enum PlayerAnims
{
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN, 
	WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN, 
	ARMED_IDLE_LEFT, ARMED_IDLE_RIGHT, ARMED_IDLE_UP, ARMED_IDLE_DOWN,
};


/* init INFO
* spritesheet loads the IDLE animations
* Creates the sprite with 16x31 pixels, scaling x2. The texture coords (UV) are 0.5 both (sprite's width and height).
* Sets 8 animations (4 facing directions + direction animation movement). 
* Spritesheet horizontal offset = 0.167 aprox (distance between sprites in spritesheet, between 0 and 1)
* To know exactly where the sprite is, we calculate offset * spritesheetColumnNum. There are 6 columns and 2 rows.
* Movement animation has 2 sprites in each direction.
*/

void Player::init(ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Solid Snake Sprites/Solid Snake Basic Animations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SPRITE_WIDTH+10, SPRITE_HEIGHT+10), glm::vec2(SPRITESHEET_OFFSET, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(IDLE_LEFT, 8);
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET * 4, 0.5f));
		
		sprite->setAnimationSpeed(IDLE_RIGHT, 8);
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 5, 0.5f));

		sprite->setAnimationSpeed(IDLE_UP, 8);
		sprite->addKeyframe(IDLE_UP, glm::vec2(SPRITESHEET_OFFSET * 5, 0.f));

		sprite->setAnimationSpeed(IDLE_DOWN, 8);
		sprite->addKeyframe(IDLE_DOWN, glm::vec2(SPRITESHEET_OFFSET * 4, 0.f));
		
		sprite->setAnimationSpeed(WALK_LEFT, 5);
		sprite->addKeyframe(WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET * 0, 0.5f));
		sprite->addKeyframe(WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET * 2, 0.5f));

		
		sprite->setAnimationSpeed(WALK_RIGHT, 5);
		sprite->addKeyframe(WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 1, 0.5f));
		sprite->addKeyframe(WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 3, 0.5f));

		sprite->setAnimationSpeed(WALK_UP, 5);
		sprite->addKeyframe(WALK_UP, glm::vec2(SPRITESHEET_OFFSET * 1, 0.f));
		sprite->addKeyframe(WALK_UP, glm::vec2(SPRITESHEET_OFFSET * 3, 0.f));

		sprite->setAnimationSpeed(WALK_DOWN, 5);
		sprite->addKeyframe(WALK_DOWN, glm::vec2(SPRITESHEET_OFFSET * 0, 0.f));
		sprite->addKeyframe(WALK_DOWN, glm::vec2(SPRITESHEET_OFFSET * 2, 0.f));

		//sprite->setAnimationSpeed(ARMED_IDLE_LEFT, 5);
		//sprite->addKeyframe(ARMED_IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET * 6, 0.5f));

		//sprite->setAnimationSpeed(ARMED_IDLE_RIGHT, 5);
		//sprite->addKeyframe(ARMED_IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 7, 0.5f));

		//sprite->setAnimationSpeed(ARMED_IDLE_UP, 5);
		//sprite->addKeyframe(ARMED_IDLE_UP, glm::vec2(SPRITESHEET_OFFSET * 6, 0.f));

		//sprite->setAnimationSpeed(ARMED_IDLE_DOWN, 5);
		//sprite->addKeyframe(ARMED_IDLE_DOWN, glm::vec2(SPRITESHEET_OFFSET * 7, 0.f));
		
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(SCREEN_WIDTH/2), float(SCREEN_HEIGHT/2)));
	punch = new Weapon();
	pause = false;
	erased = false;
	direction = 'R';
	activeObject = -1;
	cooldownKey = 0.f;
	collectAllItems = false;
}

/* WASDMovementControl INFO
* We initialize movementControl vector with all values set to false
* For each direction, set the corresponding vector position to true.
* If there is more than one position pressed, return true; otherwise return false.
*/

bool Player::WASDMovementControl()
{
	movementControl = glm::bvec4(false, false, false, false);

	if (Game::instance().getKey(GLFW_KEY_W))
		movementControl[0] = true;

	if (Game::instance().getKey(GLFW_KEY_A)) {
		movementControl[1] = true;
		if (movementControl[0])
			return true;
	}

	if (Game::instance().getKey(GLFW_KEY_S)) {
		movementControl[2] = true;
		if (movementControl[0] || movementControl[1])
			return true;

	}
	if (Game::instance().getKey(GLFW_KEY_D)) {
		movementControl[3] = true;
		if (movementControl[0] || movementControl[1] || movementControl[2])
			return true;
	}

	return false;
}

std::vector<string> Player::getActiveObjectName() const
{
	std::vector<string> properties;

	if (activeObject == -1 || objects.empty())
	{
		properties.push_back(" ");
		properties.push_back(" ");
		return properties;
	}

	if (dynamic_cast<Weapon*>(objects[activeObject]) != NULL)
	{
		properties.push_back("GUN");
		properties.push_back(" ");
	}
	else if (dynamic_cast<AccessCard*>(objects[activeObject]) != NULL)
	{
		properties.push_back("ACCESS CARD");

		AccessCard* card = dynamic_cast<AccessCard*>(objects[activeObject]);
		string num = to_string(card->getNum());
		properties.push_back(num);

		string id = to_string(card->getId());
		id = "OPENS LEVEL " + id + " DOOR";
		properties.push_back(id);
	}
	else if (dynamic_cast<Meal*>(objects[activeObject]) != NULL)
	{
		properties.push_back("MEAL");

		Meal* meal = dynamic_cast<Meal*>(objects[activeObject]);
		properties.push_back(" ");

		string healthRestored = to_string(meal->getHealthRestored());
		healthRestored = "RESTORES " + healthRestored + " HEALTH WHEN USED";
		properties.push_back(healthRestored);

	}
	else
	{
		properties.push_back(" ");
		properties.push_back(" ");
	}

	return properties;
}

/* update INFO:
* bool WASDpressed -> indicates whether a movement key (W, A, S or D) is already pressed. If true and another movement key is pressed, stops moving.
* movementControl -> (0,1,2,3) = (W,A,S,D)
*/

void Player::update(int deltaTime)
{
	if (!pause)
	{
		sprite->update(deltaTime);
		int lvl = level->getId();

		if (cooldownKey > 0)
			cooldownKey -= deltaTime;

		bool WASDpressed = WASDMovementControl();

		if (movementControl[1] && !WASDpressed)
		{
			if (sprite->animation() != WALK_LEFT)
				sprite->changeAnimation(WALK_LEFT);

			posPlayer.x -= 3;

			if (map->collisionMoveLeft(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.x += 3;
				sprite->changeAnimation(IDLE_LEFT);
			}
			direction = 'L';

		}
		else if (movementControl[3] && !WASDpressed)
		{
			if (sprite->animation() != WALK_RIGHT)
				sprite->changeAnimation(WALK_RIGHT);

			posPlayer.x += 3;

			if (map->collisionMoveRight(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.x -= 3;
				sprite->changeAnimation(IDLE_RIGHT);
			}
			direction = 'R';
		}
		else if (movementControl[0] && !WASDpressed)
		{
			if (sprite->animation() != WALK_UP)
				sprite->changeAnimation(WALK_UP);

			posPlayer.y -= 3;

			if (map->collisionMoveUp(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.y += 3;
				sprite->changeAnimation(IDLE_UP);
			}
			direction = 'U';
		}
		else if (movementControl[2] && !WASDpressed)
		{
			if (sprite->animation() != WALK_DOWN)
				sprite->changeAnimation(WALK_DOWN);

			posPlayer.y += 3;

			if (map->collisionMoveDown(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.y -= 3;
				sprite->changeAnimation(IDLE_DOWN);
			}
			direction = 'D';
		}
		else
		{
			if (sprite->animation() == WALK_LEFT)
				sprite->changeAnimation(IDLE_LEFT);
			else if (sprite->animation() == WALK_RIGHT)
				sprite->changeAnimation(IDLE_RIGHT);
			else if (sprite->animation() == WALK_UP)
				sprite->changeAnimation(IDLE_UP);
			else if (sprite->animation() == WALK_DOWN)
				sprite->changeAnimation(IDLE_DOWN);

		}

		sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));


		// KEY E -> GRABS ITMES
		if (Game::instance().getKey(GLFW_KEY_E) && cooldownKey <= 0)
		{
			cout << "TECLA E DETECTADA" << endl;
			cout << "DIRECCION -> " << direction << endl;
			glm::ivec2 centerPos = glm::ivec2(posPlayer.x + SPRITE_WIDTH / 2, posPlayer.y + SPRITE_HEIGHT - 1);
			glm::vec2 tileCoords;
			int tile = map->whichFacingTile(centerPos, direction, tileCoords);
			cout << "TILE -> " << tile << endl;
			string hide;

			if (tile == 6)
			{
				Weapon* weapon = level->getWeapon();
				objects.push_back(weapon);
				hide = "GUN";

			}
			else if (tile == 7)
			{
				AccessCard* card = level->getAccessCard();
				objects.push_back(card);
				hide = "ACCESS CARD";
			}
			else if (tile == 8)
			{
				Meal* meal = level->getMeal();
				objects.push_back(meal);
				hide = "MEAL";

			}

			cout << "VARIABLE HIDE -> " << hide << endl;
			if (!hide.empty())
			{
				level->spriteToHide(hide, tileCoords, tile);
				map = level->get_tile_map();

				if (activeObject == -1)
				{
					activeObject = 0;
					std::vector<string> properties = getActiveObjectName();
					gui->setActiveObjectName(properties[0], properties[1]);
					gui->setActiveObjectProperties(properties, properties[0]);
				}
				cooldownKey = 300.f;
			}
		}

		// KEY C -> CHANGES BETWEEN OBJECTS
		if (Game::instance().getKey(GLFW_KEY_C) && cooldownKey <= 0.f)
		{
			if (!objects.empty())
			{
				activeObject = (activeObject + 1) % objects.size();
				std::vector<string> properties = getActiveObjectName();
				gui->setActiveObjectName(properties[0], properties[1]);
				gui->setActiveObjectProperties(properties, properties[0]);
				cooldownKey = 300.f;
			}
		}

		// KEY X -> USE OBJECT
		if (Game::instance().getKey(GLFW_KEY_X) && cooldownKey <= 0.f && objects.size() > 0)
		{
			// ACCESS CARD USE
			if (dynamic_cast<AccessCard*>(objects[activeObject]) != NULL) {
				AccessCard* card = dynamic_cast<AccessCard*>(objects[activeObject]);
				glm::ivec2 centerPos = glm::ivec2(posPlayer.x + SPRITE_WIDTH / 2, posPlayer.y + SPRITE_HEIGHT - 1);
				glm::vec2 tileCoords;

				if (level->getId() == card->getId() && map->whichFacingTile(centerPos, direction, tileCoords) == 2)
					level->setDoorOpen(true);
			}
			// MEAL USE
			else if (dynamic_cast<Meal*>(objects[activeObject]) != NULL) {
				Meal* meal = dynamic_cast<Meal*>(objects[activeObject]);

				gui->updateHealth(meal->getHealthRestored());
				health += meal->getHealthRestored();
				erased = true;
			}

			if (erased)
			{
				objects.erase(objects.begin() + activeObject);

				if (objects.empty())
					activeObject = -1;
				else if (activeObject >= objects.size())
					activeObject = 0;

				std::vector<string> properties = getActiveObjectName();
				gui->setActiveObjectName(properties[0], properties[1]);
				gui->setActiveObjectProperties(properties, properties[0]);
				erased = false;
			}

			cooldownKey = 300.f;
		}

		// KEY H -> RESTORES ALL HEALTH
		if (Game::instance().getKey(GLFW_KEY_H) && cooldownKey <= 0.f)
		{
			health = gui->getMaxHealth();
			gui->updateHealth(health);
			cooldownKey = 300.f;
		}

		// KEY I -> PLAYER AUTOMATICALLY GETS ALL OBJECTS
		if (Game::instance().getKey(GLFW_KEY_I) && cooldownKey <= 0.f)
		{
			collectAllItems = true;
			cooldownKey = 300.f;
		}

		// KEY K -> TELEPORT TO FIRST INDOOR SCREEN
		if (Game::instance().getKey(GLFW_KEY_K) && cooldownKey <= 0.f)
		{
			changeMap = true;
			mapToChange = 5;
			cooldownKey = 300.f;
		}

		// KEY B -> TELEPORT TO BOSS SCREEN
		if (Game::instance().getKey(GLFW_KEY_B) && cooldownKey <= 0.f)
		{
			changeMap = true;
			mapToChange = 12;
			cooldownKey = 300.f;
		}

		// KEY G -> GOD MODE | PLAYER IS INVULNERABLE | TOGGLE TO EXIT GOD MODE
		if (Game::instance().getKey(GLFW_KEY_G) && cooldownKey <= 0.f)
		{
			cooldownKey = 300.f;
		}

		// KEY Z -> PUNCH
		if (Game::instance().getKey(GLFW_KEY_Z) && cooldownKey <= 0.f)
		{
			cooldownKey = 300.f;
		}

	}
}

bool Player::changeMap_tile(int &tileType, char &dir)
{
	glm::ivec2 centerPos = glm::ivec2(posPlayer.x + SPRITE_WIDTH / 2, posPlayer.y + SPRITE_HEIGHT - 1);
	tileType = map->whichTile(centerPos, dir);
	

	// guessing facing direction depending on the animation
	if (tileType >= 2 && tileType <= 5)
	{
		if (dir == 'O')
		{
			int currentAnim = sprite->animation();

			if (currentAnim == WALK_LEFT || currentAnim == IDLE_LEFT)
				dir = 'L';
			else if (currentAnim == WALK_RIGHT || currentAnim == IDLE_RIGHT)
				dir = 'R';
			else if (currentAnim == WALK_UP || currentAnim == IDLE_UP)
				dir = 'U';
			else if (currentAnim == WALK_DOWN || currentAnim == IDLE_DOWN)
				dir = 'D';
			else
				dir = 'D'; // default direction
		}

		if (dir != 'N')
			return true;
		direction = dir;
	}
	
	return false;
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setLevel(Level* lvl)
{
	level = lvl;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setGui(Gui* g)
{
	gui = g;
}

void Player::setLevelToGUI()
{
	int id = level->getId();
	string text = "LEVEL ";
	string idString = std::to_string(id);
	text = text + idString;

	gui->setActiveLevelName(text);
}

void Player::lookLeft()
{
	sprite->changeAnimation(IDLE_LEFT);
}

void Player::lookRight()
{
	sprite->changeAnimation(IDLE_RIGHT);
}

void Player::lookUp()
{
	sprite->changeAnimation(IDLE_UP);
}

void Player::lookDown()
{
	sprite->changeAnimation(IDLE_DOWN);
}

bool Player::hasObject(Object* object) const
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == object)
			return true;
	}
	return false;
}

void Player::addObject(Object* obj)
{
	objects.push_back(obj);
}

void Player::setActiveObject()
{
	if (activeObject == -1 && !objects.empty())
	{
		activeObject = 0;
		std::vector<string> properties = getActiveObjectName();
		gui->setActiveObjectName(properties[0], properties[1]);
		gui->setActiveObjectProperties(properties, properties[0]);
	}
}

void Player::clearAllObjects()
{
	objects.clear();
	activeObject = -1;

	// Limpia la GUI
	std::vector<string> properties;
	properties.push_back(" ");
	properties.push_back(" ");
	gui->setActiveObjectName(properties[0], properties[1]);
	gui->setActiveObjectProperties(properties, properties[0]);
}

int Player::getObjectCount() const
{
	return objects.size();
}