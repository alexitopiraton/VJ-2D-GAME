#ifndef _ACCESS_CARD_INCLUDE
#define _ACCESS_CARD_INCLUDE

#include "Object.h"

class AccessCard : public Object
{
public:
	AccessCard(int id, int level);

	void init(ShaderProgram& program) override;

	int getId() { return accessLevelId; }
	int getNum() { return num; }

private:
	int num;
	int accessLevelId;
};

#endif