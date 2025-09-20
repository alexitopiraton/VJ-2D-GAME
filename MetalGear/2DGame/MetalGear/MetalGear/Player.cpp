#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


enum PlayerAnims
{
	IDLE_LEFT, IDLE_RIGHT, IDLE_FRONT, IDLE_BACK, WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN, PUNCH
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
	sprite = Sprite::createSprite(glm::ivec2(16*2, 31*2), glm::vec2(SPRITESHEET_OFFSET, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(IDLE_LEFT, 8);
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET * 4, 0.5f));
		
		sprite->setAnimationSpeed(IDLE_RIGHT, 8);
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET * 5, 0.5f));

		sprite->setAnimationSpeed(IDLE_BACK, 8);
		sprite->addKeyframe(IDLE_BACK, glm::vec2(SPRITESHEET_OFFSET * 5, 0.f));

		sprite->setAnimationSpeed(IDLE_FRONT, 8);
		sprite->addKeyframe(IDLE_FRONT, glm::vec2(SPRITESHEET_OFFSET * 4, 0.f));
		
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
		
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
	
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

/* update INFO:
* bool WASDpressed -> indicates whether a movement key (W, A, S or D) is already pressed. If true and another movement key is pressed, stops moving.
* movementControl -> (0,1,2,3) = (W,A,S,D)
*/

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	bool WASDpressed = WASDMovementControl();

	if (movementControl[1] && !WASDpressed)
	{
		if (sprite->animation() != WALK_LEFT)
			sprite->changeAnimation(WALK_LEFT);

		posPlayer.x -= 3;
		/*if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(IDLE_LEFT);
		}*/

	}
	else if (movementControl[3] && !WASDpressed)
	{
		if (sprite->animation() != WALK_RIGHT)
			sprite->changeAnimation(WALK_RIGHT);

		posPlayer.x += 3;
		/*if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(IDLE_RIGHT);
		}*/

	}
	else if (movementControl[0] && !WASDpressed)
	{
		if (sprite->animation() != WALK_UP)
			sprite->changeAnimation(WALK_UP);

		posPlayer.y -= 3;
	}
	else if (movementControl[2] && !WASDpressed)
	{
		if (sprite->animation() != WALK_DOWN)
			sprite->changeAnimation(WALK_DOWN);

		posPlayer.y += 3;
	}
	else
	{
		if (sprite->animation() == WALK_LEFT)
			sprite->changeAnimation(IDLE_LEFT);
		else if (sprite->animation() == WALK_RIGHT)
			sprite->changeAnimation(IDLE_RIGHT);
		else if (sprite->animation() == WALK_UP)
			sprite->changeAnimation(IDLE_BACK);
		else if (sprite->animation() == WALK_DOWN)
			sprite->changeAnimation(IDLE_FRONT);

	}

	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

/*void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}*/

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}




