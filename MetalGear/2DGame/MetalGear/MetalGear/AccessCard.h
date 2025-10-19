#ifndef _ACCESS_CARD_INCLUDE
#define _ACCESS_CARD_INCLUDE

#include "Object.h"

class AccessCard : public Object
{
public:
	AccessCard();

	void init(ShaderProgram& program) override;

private:

};

#endif