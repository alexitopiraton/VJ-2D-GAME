#ifndef _MEAL_INCLUDE
#define _MEAL_INCLUDE

#include "Object.h"

class Meal : public Object
{
public:
	Meal();
	void init(ShaderProgram& program) override;

private:
	int lifeRestored;
};

#endif