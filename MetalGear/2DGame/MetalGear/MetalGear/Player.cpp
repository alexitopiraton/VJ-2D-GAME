#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "Level.h"      
#include "Guard.h"      
#include "Twin.h"       
#include "ArnoldBoss.h" 



enum PlayerAnims
{
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN, 
	WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN,
	PUNCH_LEFT, PUNCH_RIGHT, PUNCH_UP, PUNCH_DOWN,
	DEATH,
	ARMED_IDLE_LEFT, ARMED_IDLE_RIGHT, ARMED_IDLE_UP, ARMED_IDLE_DOWN,
	ARMED_WALK_LEFT, ARMED_WALK_RIGHT, ARMED_WALK_UP, ARMED_WALK_DOWN,
	DAMAGED_IDLE_LEFT, DAMAGED_IDLE_RIGHT, DAMAGED_IDLE_UP, DAMAGED_IDLE_DOWN,
	DAMAGED_WALK_LEFT, DAMAGED_WALK_RIGHT, DAMAGED_WALK_UP, DAMAGED_WALK_DOWN,
	DAMAGED_PUNCH_LEFT, DAMAGED_PUNCH_RIGHT, DAMAGED_PUNCH_UP, DAMAGED_PUNCH_DOWN,
	DAMAGED_DEATH,
	DAMAGED_ARMED_IDLE_LEFT, DAMAGED_ARMED_IDLE_RIGHT, DAMAGED_ARMED_IDLE_UP, DAMAGED_ARMED_IDLE_DOWN,
	DAMAGED_ARMED_WALK_LEFT, DAMAGED_ARMED_WALK_RIGHT, DAMAGED_ARMED_WALK_UP, DAMAGED_ARMED_WALK_DOWN
};


/* init INFO
* spritesheet loads the IDLE animations
* Creates the sprite with 16x31 pixels, scaling x2. The texture coords (UV) are 0.5 both (sprite's width and height).
* Sets 8 animations (4 facing directions + direction animation movement). 
* Spritesheet horizontal offset = 0.167 aprox (distance between sprites in spritesheet, between 0 and 1)
* To know exactly where the sprite is, we calculate offset * spritesheetColumnNum. There are 6 columns and 2 rows.
* Walk & Death animations has 2 sprites in each direction.
*/

#define SPRITESHEET_OFFSET_X 0.11111111111111111111111111111111
#define SPRITESHEET_OFFSET_Y 0.14285714285714285714285714285714

void Player::init(ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Solid Snake Sprites/Solid Snake Sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SPRITE_WIDTH+10, SPRITE_HEIGHT+10), glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(42);
	
		// IDLE ANIMATIONS
		sprite->setAnimationSpeed(IDLE_LEFT, 8);
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 4, SPRITESHEET_OFFSET_Y));
		
		sprite->setAnimationSpeed(IDLE_RIGHT, 8);
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 5, SPRITESHEET_OFFSET_Y));

		sprite->setAnimationSpeed(IDLE_UP, 8);
		sprite->addKeyframe(IDLE_UP, glm::vec2(SPRITESHEET_OFFSET_X * 5, 0.f));

		sprite->setAnimationSpeed(IDLE_DOWN, 8);
		sprite->addKeyframe(IDLE_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 4, 0.f));

		// WALK ANIMATIONS
		sprite->setAnimationSpeed(WALK_LEFT, 5);
		sprite->addKeyframe(WALK_LEFT, glm::vec2(0.f, SPRITESHEET_OFFSET_Y));
		sprite->addKeyframe(WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 2, SPRITESHEET_OFFSET_Y));
		
		sprite->setAnimationSpeed(WALK_RIGHT, 5);
		sprite->addKeyframe(WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y));
		sprite->addKeyframe(WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 3, SPRITESHEET_OFFSET_Y));

		sprite->setAnimationSpeed(WALK_UP, 5);
		sprite->addKeyframe(WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X, 0.f));
		sprite->addKeyframe(WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X * 3, 0.f));

		sprite->setAnimationSpeed(WALK_DOWN, 5);
		sprite->addKeyframe(WALK_DOWN, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(WALK_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 2, 0.f));
	
		// PUNCH ANIMATIONS
		sprite->setAnimationSpeed(PUNCH_LEFT, 8);
		sprite->addKeyframe(PUNCH_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 6, SPRITESHEET_OFFSET_Y));

		sprite->setAnimationSpeed(PUNCH_RIGHT, 8);
		sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 7, SPRITESHEET_OFFSET_Y));

		sprite->setAnimationSpeed(PUNCH_UP, 8);
		sprite->addKeyframe(PUNCH_UP, glm::vec2(SPRITESHEET_OFFSET_X * 7, 0.f));

		sprite->setAnimationSpeed(PUNCH_DOWN, 8);
		sprite->addKeyframe(PUNCH_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 6, 0.f));

		// DEATH ANIMATIONS
		sprite->setAnimationSpeed(DEATH, 4);
		sprite->addKeyframe(DEATH, glm::vec2(SPRITESHEET_OFFSET_X * 8, 0.f));
		sprite->addKeyframe(DEATH, glm::vec2(SPRITESHEET_OFFSET_X * 8, SPRITESHEET_OFFSET_Y));

		// ARMED IDLE ANIMATION
		sprite->setAnimationSpeed(ARMED_IDLE_LEFT, 8);
		sprite->addKeyframe(ARMED_IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 2, SPRITESHEET_OFFSET_Y * 5));

		sprite->setAnimationSpeed(ARMED_IDLE_RIGHT, 8);
		sprite->addKeyframe(ARMED_IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 3, SPRITESHEET_OFFSET_Y * 5));

		sprite->setAnimationSpeed(ARMED_IDLE_UP, 8);
		sprite->addKeyframe(ARMED_IDLE_UP, glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y * 5));

		sprite->setAnimationSpeed(ARMED_IDLE_DOWN, 8);
		sprite->addKeyframe(ARMED_IDLE_DOWN, glm::vec2(0.f, SPRITESHEET_OFFSET_Y * 5));

		// ARMED WALK ANIMATION
		sprite->setAnimationSpeed(ARMED_WALK_LEFT, 5);
		sprite->addKeyframe(ARMED_WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 4, SPRITESHEET_OFFSET_Y * 2));
		sprite->addKeyframe(ARMED_WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 6, SPRITESHEET_OFFSET_Y * 2));

		sprite->setAnimationSpeed(ARMED_WALK_RIGHT, 5);
		sprite->addKeyframe(ARMED_WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 5, SPRITESHEET_OFFSET_Y * 2));
		sprite->addKeyframe(ARMED_WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 7, SPRITESHEET_OFFSET_Y * 2));

		sprite->setAnimationSpeed(ARMED_WALK_UP, 5);
		sprite->addKeyframe(ARMED_WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y * 2));
		sprite->addKeyframe(ARMED_WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X * 3, SPRITESHEET_OFFSET_Y * 2));

		sprite->setAnimationSpeed(ARMED_WALK_DOWN, 5);
		sprite->addKeyframe(ARMED_WALK_DOWN, glm::vec2(0.f, SPRITESHEET_OFFSET_Y * 2));
		sprite->addKeyframe(ARMED_WALK_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 2, SPRITESHEET_OFFSET_Y * 2));

		// DAMAGED IDLE ANIMATION
		sprite->setAnimationSpeed(DAMAGED_IDLE_LEFT, 8);
		sprite->addKeyframe(DAMAGED_IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 2, SPRITESHEET_OFFSET_Y * 6));

		sprite->setAnimationSpeed(DAMAGED_IDLE_RIGHT, 8);
		sprite->addKeyframe(DAMAGED_IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 3, SPRITESHEET_OFFSET_Y * 6));

		sprite->setAnimationSpeed(DAMAGED_IDLE_UP, 8);
		sprite->addKeyframe(DAMAGED_IDLE_UP, glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y * 6));

		sprite->setAnimationSpeed(DAMAGED_IDLE_DOWN, 8);
		sprite->addKeyframe(DAMAGED_IDLE_DOWN, glm::vec2(0.f, SPRITESHEET_OFFSET_Y * 6));

		// DAMAGED WALK ANIMATION
		sprite->setAnimationSpeed(DAMAGED_WALK_LEFT, 5);
		sprite->addKeyframe(DAMAGED_WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 4, SPRITESHEET_OFFSET_Y * 3));
		sprite->addKeyframe(DAMAGED_WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 6, SPRITESHEET_OFFSET_Y * 3));

		sprite->setAnimationSpeed(DAMAGED_WALK_RIGHT, 5);
		sprite->addKeyframe(DAMAGED_WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 5, SPRITESHEET_OFFSET_Y * 3));
		sprite->addKeyframe(DAMAGED_WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 7, SPRITESHEET_OFFSET_Y * 3));

		sprite->setAnimationSpeed(DAMAGED_WALK_UP, 5);
		sprite->addKeyframe(DAMAGED_WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y * 3));
		sprite->addKeyframe(DAMAGED_WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X * 3, SPRITESHEET_OFFSET_Y * 3));

		sprite->setAnimationSpeed(DAMAGED_WALK_DOWN, 5);
		sprite->addKeyframe(DAMAGED_WALK_DOWN, glm::vec2(0.f, SPRITESHEET_OFFSET_Y * 3));
		sprite->addKeyframe(DAMAGED_WALK_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 2, SPRITESHEET_OFFSET_Y * 3));

		// DAMAGED PUNCH ANIMATION
		sprite->setAnimationSpeed(DAMAGED_PUNCH_LEFT, 8);
		sprite->addKeyframe(DAMAGED_PUNCH_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 6, SPRITESHEET_OFFSET_Y * 6));

		sprite->setAnimationSpeed(DAMAGED_PUNCH_RIGHT, 8);
		sprite->addKeyframe(DAMAGED_PUNCH_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 7, SPRITESHEET_OFFSET_Y * 6));

		sprite->setAnimationSpeed(DAMAGED_PUNCH_UP, 8);
		sprite->addKeyframe(DAMAGED_PUNCH_UP, glm::vec2(SPRITESHEET_OFFSET_X * 5, SPRITESHEET_OFFSET_Y * 6));

		sprite->setAnimationSpeed(DAMAGED_PUNCH_DOWN, 8);
		sprite->addKeyframe(DAMAGED_PUNCH_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 4, SPRITESHEET_OFFSET_Y * 6));

		// DAMAGED DEATH ANIMATION
		sprite->setAnimationSpeed(DAMAGED_DEATH, 4);
		sprite->addKeyframe(DAMAGED_DEATH, glm::vec2(SPRITESHEET_OFFSET_X * 8, SPRITESHEET_OFFSET_Y * 2));
		sprite->addKeyframe(DAMAGED_DEATH, glm::vec2(SPRITESHEET_OFFSET_X * 8, SPRITESHEET_OFFSET_Y * 3));

		// DAMAGED ARMED IDLE ANIMATION
		sprite->setAnimationSpeed(DAMAGED_ARMED_IDLE_LEFT, 8);
		sprite->addKeyframe(DAMAGED_ARMED_IDLE_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 6, SPRITESHEET_OFFSET_Y * 5));

		sprite->setAnimationSpeed(DAMAGED_ARMED_IDLE_RIGHT, 8);
		sprite->addKeyframe(DAMAGED_ARMED_IDLE_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 7, SPRITESHEET_OFFSET_Y * 5));

		sprite->setAnimationSpeed(DAMAGED_ARMED_IDLE_UP, 8);
		sprite->addKeyframe(DAMAGED_ARMED_IDLE_UP, glm::vec2(SPRITESHEET_OFFSET_X * 5, SPRITESHEET_OFFSET_Y * 5));

		sprite->setAnimationSpeed(DAMAGED_ARMED_IDLE_DOWN, 8);
		sprite->addKeyframe(DAMAGED_ARMED_IDLE_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 4, SPRITESHEET_OFFSET_Y * 5));

		// DAMAGED ARMED WALK ANIMATION
		sprite->setAnimationSpeed(DAMAGED_ARMED_WALK_LEFT, 5);
		sprite->addKeyframe(DAMAGED_ARMED_WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 4, SPRITESHEET_OFFSET_Y * 4));
		sprite->addKeyframe(DAMAGED_ARMED_WALK_LEFT, glm::vec2(SPRITESHEET_OFFSET_X * 6, SPRITESHEET_OFFSET_Y * 4));

		sprite->setAnimationSpeed(DAMAGED_ARMED_WALK_RIGHT, 5);
		sprite->addKeyframe(DAMAGED_ARMED_WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 5, SPRITESHEET_OFFSET_Y * 4));
		sprite->addKeyframe(DAMAGED_ARMED_WALK_RIGHT, glm::vec2(SPRITESHEET_OFFSET_X * 7, SPRITESHEET_OFFSET_Y * 4));

		sprite->setAnimationSpeed(DAMAGED_ARMED_WALK_UP, 5);
		sprite->addKeyframe(DAMAGED_ARMED_WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X, SPRITESHEET_OFFSET_Y * 4));
		sprite->addKeyframe(DAMAGED_ARMED_WALK_UP, glm::vec2(SPRITESHEET_OFFSET_X * 3, SPRITESHEET_OFFSET_Y * 4));

		sprite->setAnimationSpeed(DAMAGED_ARMED_WALK_DOWN, 5);
		sprite->addKeyframe(DAMAGED_ARMED_WALK_DOWN, glm::vec2(0.f, SPRITESHEET_OFFSET_Y * 4));
		sprite->addKeyframe(DAMAGED_ARMED_WALK_DOWN, glm::vec2(SPRITESHEET_OFFSET_X * 2, SPRITESHEET_OFFSET_Y * 4));

		
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(SCREEN_WIDTH/2), float(SCREEN_HEIGHT/2)));
	punch = new Weapon();
	pause = false;
	erased = false;
	direction = 'R';
	activeObject = -1;
	cooldownKey = 0.f;
	collectAllItems = false;
	godMode = false;
	fireCooldown = 200;
	isPunching = false;
	punchCooldown = 0.0f;
	punchAnimationTimer = 0.0f;
	hasWeapon = false;
	isDead = false;
	deathAnimationTimer = 0.f;
	isDamaged = false;
	damageAnimationFlashTimer = 0.f;
	damageAnimationFlashCounter = 0.f;
	showDamagedSprite = false;
	program = shaderProgram;
}

/* WASDMovementControl INFO
* We initialize movementControl vector with all values set to false
* For each direction, set the corresponding vector position to true.
* If there is more than one position pressed, return true; otherwise return false.
*/

bool Player::WASDMovementControl()
{
	movementControl = glm::bvec4(false, false, false, false);

	if (Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_UP))
		movementControl[0] = true;

	if (Game::instance().getKey(GLFW_KEY_A) || Game::instance().getKey(GLFW_KEY_LEFT)) {
		movementControl[1] = true;
		if (movementControl[0])
			return true;
	}

	if (Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_DOWN)) {
		movementControl[2] = true;
		if (movementControl[0] || movementControl[1])
			return true;

	}
	if (Game::instance().getKey(GLFW_KEY_D) || Game::instance().getKey(GLFW_KEY_RIGHT)) {
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

void Player::updateHasWeapon()
{
	bool wasArmed = hasWeapon;
	hasWeapon = false;

	if (activeObject >= 0 && activeObject < objects.size())
	{
		if (dynamic_cast<Weapon*>(objects[activeObject]) != NULL)
			hasWeapon = true;
	}

	if (wasArmed != hasWeapon && !isPunching && !isDead)
	{
		int currentAnim = sprite->animation();
		int currentKeyframe = sprite->getCurrentKeyframe();
		int newAnim = currentAnim;

		if (currentAnim == WALK_LEFT || currentAnim == ARMED_WALK_LEFT ||
			currentAnim == DAMAGED_WALK_LEFT || currentAnim == DAMAGED_ARMED_WALK_LEFT)
			newAnim = whichWalkLeftAnimation();
		else if (currentAnim == WALK_RIGHT || currentAnim == ARMED_WALK_RIGHT ||
			currentAnim == DAMAGED_WALK_RIGHT || currentAnim == DAMAGED_ARMED_WALK_RIGHT)
			newAnim = whichWalkRightAnimation();
		else if (currentAnim == WALK_UP || currentAnim == ARMED_WALK_UP ||
			currentAnim == DAMAGED_WALK_UP || currentAnim == DAMAGED_ARMED_WALK_UP)
			newAnim = whichWalkUpAnimation();
		else if (currentAnim == WALK_DOWN || currentAnim == ARMED_WALK_DOWN ||
			currentAnim == DAMAGED_WALK_DOWN || currentAnim == DAMAGED_ARMED_WALK_DOWN)
			newAnim = whichWalkDownAnimation();

		if (newAnim != currentAnim)
			sprite->changeAnimationWithKeyframe(newAnim, currentKeyframe);
	}
}

glm::vec2 Player::directionConversor()
{
	if (direction == 'R')
		return glm::vec2(1.0f, 0.0f);
	else if (direction == 'L')
		return glm::vec2(-1.0f, 0.0f);
	else if (direction == 'U')
		return glm::vec2(0.0f, -1.0f);
	else if (direction == 'D')
		return glm::vec2(0.0f, 1.0f);

}

void Player::shoot()
{
	if (timeSinceLastShot < fireCooldown)
		return;

	timeSinceLastShot = 0;

	glm::vec2 dir = directionConversor();


	glm::vec2 spawnOffset = dir * 20.0f;
	glm::vec2 bulletPos = glm::vec2(posPlayer.x + SPRITE_WIDTH / 2, posPlayer.y + SPRITE_HEIGHT / 2) + spawnOffset;

	Bullet* bullet = new Bullet(bulletPos, dir, &program, BulletType::GUARD);
	bullet->setAlive(true);
	bullets.push_back(bullet);
}

int Player::whichIdleLeftAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_IDLE_LEFT;
		return DAMAGED_IDLE_LEFT;
	}

	if (hasWeapon)
		return ARMED_IDLE_LEFT;
	return IDLE_LEFT;
}

int Player::whichIdleRightAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_IDLE_RIGHT;
		return DAMAGED_IDLE_RIGHT;
	}

	if (hasWeapon)
		return ARMED_IDLE_RIGHT;
	return IDLE_RIGHT;
}

int Player::whichIdleUpAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_IDLE_UP;
		return DAMAGED_IDLE_UP;
	}

	if (hasWeapon)
		return ARMED_IDLE_UP;
	return IDLE_UP;
}

int Player::whichIdleDownAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_IDLE_DOWN;
		return DAMAGED_IDLE_DOWN;
	}

	if (hasWeapon)
		return ARMED_IDLE_DOWN;
	return IDLE_DOWN;
}

int Player::whichWalkLeftAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_WALK_LEFT;
		return DAMAGED_WALK_LEFT;
	}

	if (hasWeapon)
		return ARMED_WALK_LEFT;

	return WALK_LEFT;
}

int Player::whichWalkRightAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_WALK_RIGHT;
		return DAMAGED_WALK_RIGHT;
	}

	if (hasWeapon)
		return ARMED_WALK_RIGHT;

	return WALK_RIGHT;
}

int Player::whichWalkUpAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_WALK_UP;
		return DAMAGED_WALK_UP;
	}

	if (hasWeapon)
		return ARMED_WALK_UP;

	return WALK_UP;
}

int Player::whichWalkDownAnimation()
{
	if (isDamaged && showDamagedSprite)
	{
		if (hasWeapon)
			return DAMAGED_ARMED_WALK_DOWN;
		return DAMAGED_WALK_DOWN;
	}

	if (hasWeapon)
		return ARMED_WALK_DOWN;

	return WALK_DOWN;
}

void Player::updateDamageAnimation()
{
	if (!isDamaged) return;

	int currentAnim = sprite->animation();
	int newAnim = currentAnim;

	bool isWalkLeft = (currentAnim == WALK_LEFT || currentAnim == DAMAGED_WALK_LEFT ||
		currentAnim == ARMED_WALK_LEFT || currentAnim == DAMAGED_ARMED_WALK_LEFT);
	bool isWalkRight = (currentAnim == WALK_RIGHT || currentAnim == DAMAGED_WALK_RIGHT ||
		currentAnim == ARMED_WALK_RIGHT || currentAnim == DAMAGED_ARMED_WALK_RIGHT);
	bool isWalkUp = (currentAnim == WALK_UP || currentAnim == DAMAGED_WALK_UP ||
		currentAnim == ARMED_WALK_UP || currentAnim == DAMAGED_ARMED_WALK_UP);
	bool isWalkDown = (currentAnim == WALK_DOWN || currentAnim == DAMAGED_WALK_DOWN ||
		currentAnim == ARMED_WALK_DOWN || currentAnim == DAMAGED_ARMED_WALK_DOWN);

	bool isIdleLeft = (currentAnim == IDLE_LEFT || currentAnim == DAMAGED_IDLE_LEFT ||
		currentAnim == ARMED_IDLE_LEFT || currentAnim == DAMAGED_ARMED_IDLE_LEFT);
	bool isIdleRight = (currentAnim == IDLE_RIGHT || currentAnim == DAMAGED_IDLE_RIGHT ||
		currentAnim == ARMED_IDLE_RIGHT || currentAnim == DAMAGED_ARMED_IDLE_RIGHT);
	bool isIdleUp = (currentAnim == IDLE_UP || currentAnim == DAMAGED_IDLE_UP ||
		currentAnim == ARMED_IDLE_UP || currentAnim == DAMAGED_ARMED_IDLE_UP);
	bool isIdleDown = (currentAnim == IDLE_DOWN || currentAnim == DAMAGED_IDLE_DOWN ||
		currentAnim == ARMED_IDLE_DOWN || currentAnim == DAMAGED_ARMED_IDLE_DOWN);

	if (isWalkLeft)
		newAnim = whichWalkLeftAnimation();
	else if (isWalkRight)
		newAnim = whichWalkRightAnimation();
	else if (isWalkUp)
		newAnim = whichWalkUpAnimation();
	else if (isWalkDown)
		newAnim = whichWalkDownAnimation();
	else if (isIdleLeft)
		newAnim = whichIdleLeftAnimation();
	else if (isIdleRight)
		newAnim = whichIdleRightAnimation();
	else if (isIdleUp)
		newAnim = whichIdleUpAnimation();
	else if (isIdleDown)
		newAnim = whichIdleDownAnimation();

	if (newAnim != currentAnim)
	{
		int currentKeyframe = sprite->getCurrentKeyframe();
		sprite->changeAnimationWithKeyframe(newAnim, currentKeyframe);
	}
}

/* update INFO:
* bool WASDpressed -> indicates whether a movement key (W, A, S or D) is already pressed. If true and another movement key is pressed, stops moving.
* movementControl -> (0,1,2,3) = (W,A,S,D)
*/

void Player::update(int deltaTime)
{
	if (isDead)
	{
		deathAnimationTimer += deltaTime;
		sprite->update(deltaTime);
		return;
	}

	if (!pause)
	{
		if (isDamaged)
		{
			damageAnimationFlashTimer += deltaTime;
			damageAnimationFlashCounter += deltaTime;

			if (damageAnimationFlashCounter >= DAMAGE_ANIMATION_FLASH_INTERVAL)
			{
				damageAnimationFlashCounter = 0.0f;
				showDamagedSprite = !showDamagedSprite;

				updateDamageAnimation();
			}

			if (damageAnimationFlashTimer >= DAMAGE_ANIMATION_FLASH_DURATION)
			{
				isDamaged = false;
				damageAnimationFlashTimer = 0.0f;
				damageAnimationFlashCounter = 0.0f;
				showDamagedSprite = false;

				updateDamageAnimation();
			}
		}

		if (punchCooldown > 0)
			punchCooldown -= deltaTime;

		if (isPunching)
		{
			punchAnimationTimer += deltaTime;
			if (punchAnimationTimer >= PUNCH_ANIMATION_DURATION)
			{
				isPunching = false;
				punchAnimationTimer = 0;

				int idleLeft = whichIdleLeftAnimation();
				int idleRight = whichIdleRightAnimation();
				int idleUp = whichIdleUpAnimation();
				int idleDown = whichIdleDownAnimation();

				if (direction == 'L')
					sprite->changeAnimation(idleLeft);
				else if (direction == 'R')
					sprite->changeAnimation(idleRight);
				else if (direction == 'U')
					sprite->changeAnimation(idleUp);
				else if (direction == 'D')
					sprite->changeAnimation(idleDown);
			}
		}

		timeSinceLastShot += deltaTime;

		for (auto it = bullets.begin(); it != bullets.end(); )
		{
			Bullet* bullet = *it;
			bullet->update(deltaTime, map);

			if (!bullet->isAlive())
			{
				delete bullet;
				it = bullets.erase(it);
			}
			else
			{
				++it;
			}
		}

		const int MAX_BULLETS = 20;
		while (bullets.size() > MAX_BULLETS)
		{
			Bullet* oldest = bullets.front();
			delete oldest;
			bullets.pop_front();
		}

		sprite->update(deltaTime);
		int lvl = level->getId();

		if (cooldownKey > 0)
			cooldownKey -= deltaTime;

		if (isPunching)
		{
			sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
			return;
		}

		bool WASDpressed = WASDMovementControl();

		updateHasWeapon();

		int idleLeft = whichIdleLeftAnimation();
		int walkLeft = whichWalkLeftAnimation();

		int idleRight = whichIdleRightAnimation();
		int walkRight = whichWalkRightAnimation();

		int idleUp = whichIdleUpAnimation();
		int walkUp = whichWalkUpAnimation();

		int idleDown = whichIdleDownAnimation();
		int walkDown = whichWalkDownAnimation();


		if (movementControl[1] && !WASDpressed)
		{
			

			if (sprite->animation() != walkLeft)
				sprite->changeAnimation(walkLeft);

			posPlayer.x -= 3;

			if (map->collisionMoveLeft(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.x += 3;
				sprite->changeAnimation(idleLeft);
			}
			direction = 'L';

		}
		else if (movementControl[3] && !WASDpressed)
		{
			if (sprite->animation() != walkRight)
				sprite->changeAnimation(walkRight);

			posPlayer.x += 3;

			if (map->collisionMoveRight(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.x -= 3;
				sprite->changeAnimation(idleRight);
			}
			direction = 'R';
		}
		else if (movementControl[0] && !WASDpressed)
		{
			if (sprite->animation() != walkUp)
				sprite->changeAnimation(walkUp);

			posPlayer.y -= 3;

			if (map->collisionMoveUp(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.y += 3;
				sprite->changeAnimation(idleUp);
			}
			direction = 'U';
		}
		else if (movementControl[2] && !WASDpressed)
		{
			if (sprite->animation() != walkDown)
				sprite->changeAnimation(walkDown);

			posPlayer.y += 3;

			if (map->collisionMoveDown(posPlayer, glm::ivec2(SPRITE_WIDTH, SPRITE_HEIGHT), lvl))
			{
				posPlayer.y -= 3;
				sprite->changeAnimation(idleDown);
			}
			direction = 'D';
		}
		else
		{
			if (sprite->animation() == walkLeft)
				sprite->changeAnimation(idleLeft);
			else if (sprite->animation() == walkRight)
				sprite->changeAnimation(idleRight);
			else if (sprite->animation() == walkUp)
				sprite->changeAnimation(idleUp);
			else if (sprite->animation() == walkDown)
				sprite->changeAnimation(idleDown);

		}

		sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));


		// KEY E -> GRABS ITMES
		if (Game::instance().getKey(GLFW_KEY_E) && cooldownKey <= 0)
		{
			glm::ivec2 centerPos = glm::ivec2(posPlayer.x + SPRITE_WIDTH / 2, posPlayer.y + SPRITE_HEIGHT - 1);
			glm::vec2 tileCoords;
			int tile = map->whichFacingTile(centerPos, direction, tileCoords);
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
			if (dynamic_cast<AccessCard*>(objects[activeObject]) != NULL) 
			{
				AccessCard* card = dynamic_cast<AccessCard*>(objects[activeObject]);
				glm::ivec2 centerPos = glm::ivec2(posPlayer.x + SPRITE_WIDTH / 2, posPlayer.y + SPRITE_HEIGHT - 1);
				glm::vec2 tileCoords;

				if (level->getId() == card->getId() && map->whichFacingTile(centerPos, direction, tileCoords) == 2)
					level->setDoorOpen(true);
			}
			// MEAL USE
			else if (dynamic_cast<Meal*>(objects[activeObject]) != NULL)
			{
				Meal* meal = dynamic_cast<Meal*>(objects[activeObject]);

				gui->updateHealth(meal->getHealthRestored());
				health += meal->getHealthRestored();
				erased = true;
			}

			// GUN USE
			else if (dynamic_cast<Weapon*>(objects[activeObject]) != NULL)
			{
				shoot();
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

		// KEY G -> GOD MODE
		if (Game::instance().getKey(GLFW_KEY_G) && cooldownKey <= 0.f)
		{
			godMode = !godMode;
			cooldownKey = 300.f;
		}

		// KEY Z -> PUNCH
		if (Game::instance().getKey(GLFW_KEY_Z) && punchCooldown <= 0.f && !isPunching)
		{
			performPunch();
		}

	}
}

void Player::performPunch()
{
	isPunching = true;
	punchAnimationTimer = 0;
	punchCooldown = PUNCH_COOLDOWN_TIME;

	if (direction == 'L')
		sprite->changeAnimation(PUNCH_LEFT);
	else if (direction == 'R')
		sprite->changeAnimation(PUNCH_RIGHT);
	else if (direction == 'U')
		sprite->changeAnimation(PUNCH_UP);
	else if (direction == 'D')
		sprite->changeAnimation(PUNCH_DOWN);

	glm::ivec2 punchHitbox = getPunchHitbox();

	checkPunchCollisions(punchHitbox);
}

glm::ivec2 Player::getPunchHitbox()
{
	const int PUNCH_RANGE = 40;
	glm::ivec2 hitboxPos = posPlayer;

	if (direction == 'L')
		hitboxPos.x -= PUNCH_RANGE;
	else if (direction == 'R')
		hitboxPos.x += SPRITE_WIDTH;
	else if (direction == 'U')
		hitboxPos.y -= PUNCH_RANGE;
	else if (direction == 'D')
		hitboxPos.y += SPRITE_HEIGHT;

	return hitboxPos;
}

void Player::checkPunchCollisions(const glm::ivec2& punchPos)
{
	const int PUNCH_RANGE = 40;
	const int PUNCH_DAMAGE = 10;

	glm::ivec2 punchSize(PUNCH_RANGE, PUNCH_RANGE);

	for (Guard* guard : level->getGuards())
	{
		if (guard == nullptr || !guard->isAlive()) continue;

		glm::vec2 guardPos = guard->getPosition();
		glm::ivec2 guardSize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

		bool collisionX = punchPos.x + punchSize.x >= guardPos.x &&
			guardPos.x + guardSize.x >= punchPos.x;
		bool collisionY = punchPos.y + punchSize.y >= guardPos.y &&
			guardPos.y + guardSize.y >= punchPos.y;

		if (collisionX && collisionY)
		{
			guard->takeDamage(PUNCH_DAMAGE);
		}
	}

	if (level->getTwin() != nullptr && level->getTwin()->isAlive())
	{
		glm::vec2 twinPos = level->getTwin()->getPosition();
		glm::ivec2 twinSize(SPRITE_WIDTH + 30, SPRITE_HEIGHT + 10);

		bool collisionX = punchPos.x + punchSize.x >= twinPos.x &&
			twinPos.x + twinSize.x >= punchPos.x;
		bool collisionY = punchPos.y + punchSize.y >= twinPos.y &&
			twinPos.y + twinSize.y >= punchPos.y;

		if (collisionX && collisionY)
		{
			level->getTwin()->takeDamage(PUNCH_DAMAGE);
		}
	}

	if (level->getArnoldBoss() != nullptr && !level->getArnoldBoss()->getIsDead())
	{
		glm::vec2 arnoldPos = level->getArnoldBoss()->getPosition();
		glm::ivec2 arnoldSize(64, 64);

		bool collisionX = punchPos.x + punchSize.x >= arnoldPos.x &&
			arnoldPos.x + arnoldSize.x >= punchPos.x;
		bool collisionY = punchPos.y + punchSize.y >= arnoldPos.y &&
			arnoldPos.y + arnoldSize.y >= punchPos.y;

		if (collisionX && collisionY)
		{
			level->getArnoldBoss()->takeDamage(PUNCH_DAMAGE);
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
				dir = 'D';
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
	for (Bullet* b : bullets)
	{
		b->render();
	}
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
	int idleLeft = whichIdleLeftAnimation();
	sprite->changeAnimation(idleLeft);
}

void Player::lookRight()
{
	int idleRight = whichIdleRightAnimation();
	sprite->changeAnimation(idleRight);
}

void Player::lookUp()
{
	int idleUp = whichIdleUpAnimation();
	sprite->changeAnimation(idleUp);
}

void Player::lookDown()
{
	int idleDown = whichIdleDownAnimation();
	sprite->changeAnimation(idleDown);
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

void Player::takeDamage(int dmg)
{
	if (!godMode && !isDead)
	{
		health -= dmg;
		gui->updateHealth(-dmg);

		isDamaged = true;
		damageAnimationFlashTimer = 0.0f;
		damageAnimationFlashCounter = 0.0f;
		showDamagedSprite = true; 

		updateDamageAnimation();
	}

	if (health < 0) health = 0;


	if (health <= 0 && !isDead)
	{
		isDead = true;
		deathAnimationTimer = 0.f;
		health = 0;

		sprite->changeAnimation(DEATH);

	}
}

void Player::clearBullets()
{
	for (Bullet* b : bullets) {
		delete b;
	}
	bullets.clear();
}

void Player::reset()
{
	// Reset player
	health = gui->getMaxHealth();
	pause = false;
	direction = 'R';
	cooldownKey = 0.f;
	godMode = false;
	timeSinceLastShot = 0;
	hasWeapon = false;

	isPunching = false;
	punchCooldown = 0.0f;
	punchAnimationTimer = 0.0f;

	isDead = false;
	deathAnimationTimer = 0.f;

	isDamaged = false;
	damageAnimationFlashTimer = 0.0f;
	damageAnimationFlashCounter = 0.0f;
	showDamagedSprite = false;

	// Reset bullets
	clearBullets();

	// Reset GUI
	gui->updateHealth(health);
	clearAllObjects();

	// Reset objects
	activeObject = -1;
	collectAllItems = false;
	erased = false;

	sprite->changeAnimation(IDLE_RIGHT);
}

int Player::getWeaponDamage()
{
	Weapon* weapon = dynamic_cast<Weapon*>(objects[activeObject]);
	return weapon->getDamage();
}

