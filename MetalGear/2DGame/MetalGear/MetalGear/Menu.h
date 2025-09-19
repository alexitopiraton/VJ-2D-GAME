#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "ShaderProgram.h"

class Menu
{
public:
    Menu();
    ~Menu();

    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void free();

    bool isStartPressed() const;
    void resetStartPressed();

private:
    bool startPressed;
};

#endif // _MENU_INCLUDE