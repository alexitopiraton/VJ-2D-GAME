#include "Object.h"

Object::Object()
{
	sprite = NULL;
}

Object::~Object()
{
	if (sprite != NULL)
		delete sprite;
}

void Object::render()
{
	sprite->render();
}

void Object::setPosition(glm::ivec2 newPos)
{
	posObject = newPos;
	sprite->setPosition(glm::vec2(posObject.x, posObject.y));
}

glm::ivec2 Object::getPosition() const
{
	return posObject;
}