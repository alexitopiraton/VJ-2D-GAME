#include "Weapon.h"

Weapon::Weapon()
{

}

void Weapon::init(ShaderProgram& program)
{
	spriteImage.loadFromFile("images/Objects/gun.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(30, 20), glm::vec2(1.f, 1.f), &spriteImage, &program);

	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);

	dmg = 10;
}