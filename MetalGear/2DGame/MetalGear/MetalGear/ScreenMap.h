#ifndef _SCREENMAP_INCLUDE
#define _SCREENMAP_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"

class ScreenMap
{
public:
    ScreenMap();
    ~ScreenMap();

    void init(const glm::ivec2& imageSize, const glm::vec2& position, ShaderProgram& shaderProgram);
    void loadImage(const string& imagePath);
    void render();

    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;

private:
    Texture mapImage;
    Sprite* sprite;
    glm::vec2 posScreenMap;
};

#endif // _SCREENMAP_INCLUDE