#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"
#include "Texture.h"
#include <iostream>


class Object
{
public:
	Object();
	virtual ~Object();

	virtual void init(ShaderProgram& program) = 0;	// abstract function

	void render();
	void setPosition(glm::ivec2 newPos);
	glm::ivec2 getPosition() const;

// protected as Object is an abstract class. Subclasses need to have access to sprite
protected:
	Sprite* sprite;
	Texture spriteImage;
	glm::ivec2 posObject;
};

#endif