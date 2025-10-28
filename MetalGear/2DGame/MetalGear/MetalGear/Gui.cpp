#include "Gui.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Gui::Gui()
{
    guiSprite = NULL;
    heartSprite = NULL;
    characters = NULL;

    maxHealth = 100;
    currentHealth = 0;
}

Gui::~Gui()
{
    if (guiSprite != NULL)
        delete guiSprite;
    if (heartSprite != NULL)
        delete heartSprite;
    if (characters != NULL)
        delete characters;
}

void Gui::init(ShaderProgram& program)
{
    texProgram = program;
    projection = glm::ortho(0.f, float(GUI_WIDTH), float(GUI_HEIGHT), 0.f);

    // GUI SPRITE
    guiTexture.loadFromFile("images/GUI/gui.png", TEXTURE_PIXEL_FORMAT_RGBA);
    guiSprite = Sprite::createSprite(glm::ivec2(GUI_WIDTH, GUI_HEIGHT), glm::vec2(1.0f, 1.0f), &guiTexture, &program);

    guiSprite->setNumberAnimations(1);
    guiSprite->setAnimationSpeed(0, 1);
    guiSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    guiSprite->changeAnimation(0);

    guiSprite->setPosition(glm::vec2(0.f, 0.f));

    // HEART SPRITE
    heartTexture.loadFromFile("images/GUI/heart1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    heartSprite = Sprite::createSprite(glm::ivec2(13*3, 11*3), glm::vec2(1.0f, 1.0f), &heartTexture, &program);

    heartSprite->setNumberAnimations(1);
    heartSprite->setAnimationSpeed(0, 1);
    heartSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    heartSprite->changeAnimation(0);

    heartSprite->setPosition(glm::vec2(10.f, 10.f));

    // LIFEBAR
    lifeBarTexture.loadFromFile("images/GUI/lifeBar.png", TEXTURE_PIXEL_FORMAT_RGBA);
    lifeBarSprite = Sprite::createSprite(glm::ivec2(200, 17), glm::vec2(1.0f, 1.0f), &lifeBarTexture, &program);

    lifeBarSprite->setNumberAnimations(1);
    lifeBarSprite->setAnimationSpeed(0, 1);
    lifeBarSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    lifeBarSprite->changeAnimation(0);

    lifeBarSprite->setPosition(glm::vec2(13 * 3 + 20.f, 20.f));

    // TEXT CONFIGURATION
    characters = new Characters();
    characters->init(texProgram);

    characters->addText("active_object", " ", glm::vec2(GUI_WIDTH - 10, 10.f));
    characters->addText("level", "LEVEL 0", glm::vec2(10.f, GUI_HEIGHT - 30));
    characters->addText("active object property 1", " ", glm::vec2(GUI_WIDTH - 10, 40.f));
    characters->addText("active object property 2", " ", glm::vec2(GUI_WIDTH - 10, 70.f));
}

void Gui::update(int deltaTime)
{
    // Aquí puedes actualizar elementos animados de la GUI si los necesitas
    if (guiSprite != NULL)
        guiSprite->update(deltaTime);
}

void Gui::render()
{
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    if (guiSprite != NULL)
    {
        guiSprite->render();
    }

    heartSprite->render();
    renderLifeBar();
    characters->render();
}

void Gui::setActiveObjectName(const string& objectName, const string& count)
{
    string name = objectName + " " + count;
    float textWidth = name.length() * 20.f;
    float posX = GUI_WIDTH - textWidth - 10.f;

    

    characters->updateText("active_object", name);
    characters->updatePosition("active_object", glm::vec2(posX, 10.f));
}

void Gui::setActiveLevelName(const string& levelName)
{
    characters->updateText("level", levelName);
}

void Gui::setActiveObjectProperties(const std::vector<string>& properties, const string& type)
{
    characters->hideText("active object property 1");
    characters->hideText("active object property 2");

    for (int i = 2; i < properties.size(); i++)
    {
        string property = properties[i];
        float posX = 10.f + property.size() * 20.f;

        if (i == 2)
        {
            characters->updateText("active object property 1", property);
            characters->updatePosition("active object property 1", glm::vec2(GUI_WIDTH - posX, 40.f));
            characters->showText("active object property 1");
        }
        else if (i == 3)
        {
            characters->updateText("active object property 2", property);
            characters->updatePosition("active object property 2", glm::vec2(GUI_WIDTH - posX, 70.f));
            characters->showText("active object property 2");
        }
    }
}

/* renderLifeBar 
*/
void Gui::renderLifeBar()
{
    // Calcular el ancho de la barra según la vida actual
    float lifePercentage = (float)currentHealth / (float)maxHealth;
    int barWidth = (int)(200 * lifePercentage);
    int barX = 13 * 3 + 20;
    int barY = 20;
    int windowY = GUI_HEIGHT - barY - 17;

    // Usar scissor test para recortar la barra
    glEnable(GL_SCISSOR_TEST);
    glScissor(barX, windowY, barWidth, 17); // x, y (desde abajo), width, height

    lifeBarSprite->render();

    glDisable(GL_SCISSOR_TEST);
}

void Gui::setMaxHealth(const int& health)
{
    maxHealth = health;
}

void Gui::updateHealth(const int& healthValue)
{
    currentHealth += healthValue;

    if (currentHealth > maxHealth)
        currentHealth = maxHealth;
}

void Gui::reset()
{
    currentHealth = maxHealth;

    characters->updateText("active_object", " ");
    characters->updateText("active object property 1", " ");
    characters->updateText("active object property 2", " ");
    characters->hideText("active object property 1");
    characters->hideText("active object property 2");

    characters->updateText("level", "LEVEL 3");
}