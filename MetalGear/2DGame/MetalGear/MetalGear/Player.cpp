#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"



enum PlayerAnims
{
	IDLE_LEFT, IDLE_RIGHT, IDLE_BACK, IDLE_FRONT, MOVE_LEFT, MOVE_RIGHT
};


/* init INFO
* spritesheet loads the IDLE animations
* Creates the sprite with 16x31 pixels, scaling x2. The texture coords (UV) are 0.5 both.
* Sets 4 animations (4 facing directions).
* STAND_LEFT (0,0.5) 
*/

void Player::init(ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Solid Snake Idle.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16*2, 31*2), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(IDLE_LEFT, 8);
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(IDLE_RIGHT, 8);
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(IDLE_BACK, 8);
		sprite->addKeyframe(IDLE_BACK, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(IDLE_FRONT, 8);
		sprite->addKeyframe(IDLE_FRONT, glm::vec2(0.f, 0.f));
		
		/*sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));*/
		
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
		//if (sprite->animation() != MOVE_LEFT)
			//sprite->changeAnimation(MOVE_LEFT);
		sprite->changeAnimation(IDLE_LEFT);
		posPlayer.x--;
		/*if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(IDLE_LEFT);
		}*/

	}
	else if (movementControl[3] && !WASDpressed)
	{
		//if (sprite->animation() != MOVE_RIGHT)
			//sprite->changeAnimation(MOVE_RIGHT);
		sprite->changeAnimation(IDLE_RIGHT);
		posPlayer.x++;
		/*if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(IDLE_RIGHT);
		}*/

	}
	else if (movementControl[0] && !WASDpressed)
	{
		sprite->changeAnimation(IDLE_BACK);
		posPlayer.y--;
	}
	else if (movementControl[2] && !WASDpressed)
	{
		sprite->changeAnimation(IDLE_FRONT);
		posPlayer.y++;
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(IDLE_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(IDLE_RIGHT);

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




