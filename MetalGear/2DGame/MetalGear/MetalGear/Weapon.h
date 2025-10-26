#ifndef _WEAPON_INCLUDE
#define _WEAPON_INCLUDE

#include "Object.h"

class Weapon : public Object
{
public:
	Weapon();

	void init(ShaderProgram& program) override;

	int getDamage() { return dmg; }
	
private:
	int dmg;
};

#endif