#include "Level.h"

Level::Level()
{
	map = NULL;
	background = NULL;
	blackScreen = NULL;
	openedVersion = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (background != NULL)
		delete background;
	if (blackScreen != NULL)
		delete blackScreen;
	if (openedVersion != NULL)
		delete openedVersion;
}

TileMap* Level::get_tile_map()
{
	return map;
}

void Level::init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet, const std::vector<string>& objectTypes, const std::vector<std::pair<int,int>>& objectPositions, const int& num, const std::vector<int>& accessCardNumbers, int &iterator)
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

	// Opened version image
	if (num == 3)
	{
		openedVersionImage.loadFromFile("images/Map Changes/outsideScreen4Opened.png", TEXTURE_PIXEL_FORMAT_RGBA);
		openedVersion = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.f, 1.f), &openedVersionImage, &program);
		openedVersion->setNumberAnimations(1);
		openedVersion->setAnimationSpeed(0, 1);
		openedVersion->addKeyframe(0, glm::vec2(0.f, 0.f));
		openedVersion->changeAnimation(0);
	}
	else if (num == 4)
	{
		openedVersionImage.loadFromFile("images/Map Changes/transitionScreenOpened.png", TEXTURE_PIXEL_FORMAT_RGBA);
		openedVersion = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.f, 1.f), &openedVersionImage, &program);
		openedVersion->setNumberAnimations(1);
		openedVersion->setAnimationSpeed(0, 1);
		openedVersion->addKeyframe(0, glm::vec2(0.f, 0.f));
		openedVersion->changeAnimation(0);
	}
	else if (num == 6)
	{
		openedVersionImage.loadFromFile("images/Map Changes/indoorScreen2Opened.png", TEXTURE_PIXEL_FORMAT_RGBA);
		openedVersion = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.f, 1.f), &openedVersionImage, &program);
		openedVersion->setNumberAnimations(1);
		openedVersion->setAnimationSpeed(0, 1);
		openedVersion->addKeyframe(0, glm::vec2(0.f, 0.f));
		openedVersion->changeAnimation(0);
	}
	else if (num == 11)
	{
		openedVersionImage.loadFromFile("images/Map Changes/indoorScreen7Opened.png", TEXTURE_PIXEL_FORMAT_RGBA);
		openedVersion = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.f, 1.f), &openedVersionImage, &program);
		openedVersion->setNumberAnimations(1);
		openedVersion->setAnimationSpeed(0, 1);
		openedVersion->addKeyframe(0, glm::vec2(0.f, 0.f));
		openedVersion->changeAnimation(0);
	}

	// Objects

	weapon = NULL;
	meal = NULL;
	accessCard = NULL;

	hasWeaponOriginal = false;
	hasAccessCardOriginal = false;
	hasMealOriginal = false;

	for (int i = 0; i < objectTypes.size(); i++)
	{

		string type = objectTypes[i];
		glm::ivec2 position = glm::ivec2(objectPositions[i].first, objectPositions[i].second);
		
		if (type == "MEAL")
		{
			meal = new Meal();
			meal->init(program);
			meal->setPosition(position);
			mealOriginalPos = position;
			hasMealOriginal = true;
		}
		else if (type == "ACCESS_CARD")
		{
			accessCard = new AccessCard(iterator+1, accessCardNumbers[iterator]);
			accessCard->init(program);
			accessCard->setPosition(position);
			accessCardOriginalPos = position;
			accessCardNum = iterator + 1;
			accessCardId = accessCardNumbers[iterator];
			hasAccessCardOriginal = true;
			iterator++;
		}
		else if (type == "WEAPON")
		{
			weapon = new Weapon();
			weapon->init(program);
			weapon->setPosition(position);
			weaponOriginalPos = position;
			hasWeaponOriginal = true;
		}
	}

	// Other attributes
	pause = false;
	hideWeapon = false;
	hideAccessCard = false;
	hideMeal = false;
	id = num;
	numEnemies = 0;
	doorOpened = false;
}

void Level::render()
{
	if (!pause)
	{
		map->render();

		if ((doorOpened && openedVersion != NULL) || (numEnemies == 0 && id == 3))
			openedVersion->render();
		else
			background->render();


		if (meal != NULL && !hideMeal)
			meal->render();

		if (accessCard != NULL && !hideAccessCard)
			accessCard->render();

		if (weapon != NULL && !hideWeapon)
			weapon->render();
	}
		
}

void Level::spriteToHide(const string& hide, const glm::vec2 &tileCoords, const int& tile)
{
	if (hide == "MEAL")
	{
		hideMeal = true;
	}
	else if (hide == "ACCESS CARD")
	{
		hideAccessCard = true;
	}
	else if (hide == "GUN")
	{
		hideWeapon = true;
	}

	if(id != 7)
		map->changeTile(tileCoords, tile);
}

void Level::setBlackScreen()
{
	blackScreen->render();
}

void Level::setDoorOpen(const bool& open)
{ 
	doorOpened = open;
	map->setDoorOpen(open); 
}

bool Level::hasWeapon() const
{
	return weapon != NULL && !hideWeapon;
}

bool Level::hasAccessCard() const
{
	return accessCard != NULL && !hideAccessCard;
}

bool Level::hasMeal() const
{
	return meal != NULL && !hideMeal;
}

void Level::collectAllObjects()
{
	if (weapon != NULL)
		hideWeapon = true;
	if (accessCard != NULL)
		hideAccessCard = true;
	if (meal != NULL)
		hideMeal = true;

	if (id != 7) 
	{
		map->removeObjectTiles();
	}
}

void Level::hideObjectWeapon()
{
	hideWeapon = true;
}

void Level::hideObjectAccessCard()
{
	hideAccessCard = true;
}

void Level::hideObjectMeal()
{
	hideMeal = true;
}

void Level::regenerateWeapon(ShaderProgram& program)
{
	if (!hasWeaponOriginal)
		return;

	if (weapon == NULL)
	{
		weapon = new Weapon();
		weapon->init(program);
		weapon->setPosition(weaponOriginalPos);
	}
	hideWeapon = false;
}

void Level::regenerateAccessCard(ShaderProgram& program)
{
	if (!hasAccessCardOriginal)
		return;

	if (accessCard == NULL)
	{
		accessCard = new AccessCard(accessCardNum, accessCardId);
		accessCard->init(program);
		accessCard->setPosition(accessCardOriginalPos);
	}
	hideAccessCard = false;
}

void Level::regenerateMeal(ShaderProgram& program)
{
	if (!hasMealOriginal)
		return;

	if (meal == NULL)
	{
		meal = new Meal();
		meal->init(program);
		meal->setPosition(mealOriginalPos);
	}
	hideMeal = false;
}