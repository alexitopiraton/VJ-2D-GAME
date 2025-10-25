#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "Sprite.h"
#include "Texture.h"
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
    int getSelectedOption() const;
    void resetStartPressed();

private:

    void updateCursorPosition();

    bool startPressed;
    int selectedOption;

    Texture menuTexture;
    Sprite* menuSprite;
    Sprite* cursorSprite;
    Texture cursorTexture;
    ShaderProgram* shaderProgram;

    // Posiciones del cursor
    glm::vec2 startPosition;
    glm::vec2 continuePosition;

    float blinkTime;
    float blinkInterval;
    bool cursorVisible;
};

#endif // _MENU_INCLUDE