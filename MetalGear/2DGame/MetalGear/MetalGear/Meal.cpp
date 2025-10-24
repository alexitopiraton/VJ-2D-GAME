#include "Meal.h"

Meal::Meal()
{

}

void Meal::init(ShaderProgram& program)
{
	spriteImage.loadFromFile("images/Objects/meal.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(30, 30), glm::vec2(1.f, 1.f), &spriteImage, &program);

	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);

	healthRestored = 25;
}