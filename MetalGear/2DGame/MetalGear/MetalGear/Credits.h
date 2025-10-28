#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include "Sprite.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Characters.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Credits
{
public:
    Credits();
    ~Credits();

    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void reset();

    bool isFinished() const { return creditsFinished; }

private:
    struct CreditLine {
        std::string id;
        std::string text;
        glm::vec2 initialPosition;
        float delay;
        bool visible;
    };

    ShaderProgram* shaderProgram;
    Characters characters;

    std::vector<CreditLine> creditLines;

    float currentTime;
    bool creditsFinished;

    float scrollSpeed;
    float scrollOffset;

    bool musicStarted;
    float gameAreaY;
    float gameAreaHeight;

    Texture creditsTexture;
    Sprite* creditsSprite;

};

#endif