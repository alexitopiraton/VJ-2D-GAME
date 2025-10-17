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

void Object::init()
{

}