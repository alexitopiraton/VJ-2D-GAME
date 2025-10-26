#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Texture.h"
#include "Characters.h"

class Gui
{
public:
    Gui();
    ~Gui();

    void init(ShaderProgram& program);
    void render();
    void update(int deltaTime);

    int getMaxHealth() { return maxHealth; }

    void setActiveObjectName(const string& objectName, const string& count);
    void setActiveLevelName(const string& levelName);
    void setMaxHealth(const int& health);
    void setActiveObjectProperties(const std::vector<string>& properties, const string& type);

    void updateHealth(const int& healthValue);
    void renderLifeBar();

    void reset();

private:
    Texture guiTexture, heartTexture;
    Sprite* guiSprite;
    Sprite* heartSprite;
    glm::mat4 projection;
    ShaderProgram texProgram;
    Characters* characters;

    Sprite* lifeBarSprite;
    Texture lifeBarTexture;
    int maxHealth;
    int currentHealth;
};

#endif // _GUI_INCLUDE