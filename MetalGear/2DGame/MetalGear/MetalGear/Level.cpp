#include "Level.h"

Level::Level()
{
	map = NULL;
	background = NULL;
	blackScreen = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (background != NULL)
		delete background;
	if (blackScreen != NULL)
		delete blackScreen;
}

TileMap* Level::get_tile_map()
{
	return map;
}

void Level::init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet, const std::vector<string>& objectTypes, const std::vector<std::pair<int,int>>& objectPositions)
{

	// Tile Map
	map = TileMap::createTileMap(levelFile, minCoords, program);

	// Map image
	backgroundImage.loadFromFile(backgroundFile, TEXTURE_PIXEL_FORMAT_RGBA);
	if(outside)
		background = Sprite::createSprite(glm::ivec2(640,480), glm::vec2(0.5f, IMAGE_OFFSET), &backgroundImage, &program);
	else
		background = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(IMAGE_OFFSET, IMAGE_OFFSET), &backgroundImage, &program);

	background->setNumberAnimations(1);
	background->setAnimationSpeed(0, 1);
	background->addKeyframe(0, positionInSpritesheet);
	background->changeAnimation(0);

	// Black screen image 
	blackScreenImage.loadFromFile("images/black_screen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	blackScreen = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.f, 1.f), &blackScreenImage, &program);
	blackScreen->setNumberAnimations(1);
	blackScreen->setAnimationSpeed(0, 1);
	blackScreen->addKeyframe(0, glm::vec2(0.f, 0.f));
	blackScreen->changeAnimation(0);

	// Objects

	weapon = NULL;
	meal = NULL;
	accessCard = NULL;

	for (int i = 0; i < objectTypes.size(); i++)
	{

		string type = objectTypes[i];
		glm::ivec2 position = glm::ivec2(objectPositions[i].first, objectPositions[i].second);
		
		if (type == "MEAL")
		{
			meal = new Meal();
			meal->init(program);
			meal->setPosition(position);
		}
		else if (type == "ACCESS_CARD")
		{
			accessCard = new AccessCard();
			accessCard->init(program);
			accessCard->setPosition(position);
		}
		else if (type == "WEAPON")
		{
			weapon = new Weapon();
			weapon->init(program);
			weapon->setPosition(position);
		}
	}

	// Other attributes
	pause = false;
}

void Level::render()
{
	if (!pause)
	{
		map->render();
		//background->render();

		if (meal != NULL)
			meal->render();

		if (accessCard != NULL)
			accessCard->render();

		if (weapon != NULL)
			weapon->render();
	}
		
}

void Level::setBlackScreen()
{
	blackScreen->render();
}
