#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"


class Object
{
public:
	Object();
	~Object();

	void init();

private:
	Sprite* sprite;
};

#endif