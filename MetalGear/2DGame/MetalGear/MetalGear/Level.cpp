#include "Level.h"
#include <iostream>
#include "Twin.h"

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

	// Liberar guardias
	for (Guard* guard : guards)
		delete guard;
	guards.clear();
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
	hideWeapon = false;
	hideAccessCard = false;
	hideMeal = false;
}

void Level::addGuard(const glm::vec2& position, ShaderProgram& program)
{
	Guard* guard = new Guard();
	guard->init(program);
	guard->setShaderProgram(&program);
	guard->setPosition(position);
	guards.push_back(guard);
}

void Level::update(int deltaTime, Player* player)
{
	updateRollers(deltaTime, player);

	if(arnoldBoss)
		arnoldBoss->update(deltaTime, *map, *player);


	if (twin) {
		twin->AIControl(*map, *player, deltaTime);
		twin->update(deltaTime);
		const std::list<Bullet*>& bullets = twin->getBullets();

		for (Bullet* b : bullets)
		{
			if (b->isAlive()) {
				glm::vec2 bulletPos = b->getPosition();
				glm::ivec2 bulletSize(16, 16);
				glm::ivec2 playerSize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

				bool collisionX = bulletPos.x + bulletSize.x >= player->getPosition().x &&
					player->getPosition().x + playerSize.x >= bulletPos.x;
				bool collisionY = bulletPos.y + bulletSize.y >= player->getPosition().y &&
					player->getPosition().y + playerSize.y >= bulletPos.y;

				if (collisionX && collisionY) // radio de colisión
				{
					player->takeDamage(20);
					b->setAlive(false);
				}
			}
		}


	}

	// Actualizar todos los guardias
	for (Guard* guard : guards)
	{
		guard->AIControl(*map, *player, deltaTime);
		guard->update(deltaTime);

		// --- Comprobar colisiones de balas del guardia con el jugador ---
		const std::list<Bullet*>& bullets = guard->getBullets();



		for (Bullet* b : bullets)
		{
			if (b->isAlive()) {
				glm::vec2 bulletPos = b->getPosition();
				glm::ivec2 bulletSize(16, 16);
				glm::ivec2 playerSize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

				bool collisionX = bulletPos.x + bulletSize.x >= player->getPosition().x &&
					player->getPosition().x + playerSize.x >= bulletPos.x;
				bool collisionY = bulletPos.y + bulletSize.y >= player->getPosition().y &&
					player->getPosition().y + playerSize.y >= bulletPos.y;

				if (collisionX && collisionY) // radio de colisión
				{
					player->takeDamage(20);
					b->setAlive(false);
				}
			}
		}
	}
}

void Level::render()
{
	if (!pause)
	{
		map->render();
		background->render();

		if (meal != NULL && !hideMeal)
			meal->render();

		if (accessCard != NULL && !hideAccessCard)
			accessCard->render();

		if (weapon != NULL && !hideWeapon)
			weapon->render();
    
      // Renderizar guardias
    for (Guard* guard : guards)
      guard->render();

    renderRollers();
    if(arnoldBoss)
      arnoldBoss->render();

    if (twin) {
      twin->render();
    }
	}
		
}

void Level::spriteToHide(const string& hide, const glm::vec2 &tileCoords, const int& tile)
{
	if (hide == "MEAL")
	{
		hideMeal = true;
	}
	else if (hide == "ACCESS_CARD")
	{
		hideAccessCard = true;
	}
	else if (hide == "WEAPON")
	{
		hideWeapon = true;
	}

	map->changeTile(tileCoords, tile);
}

void Level::setBlackScreen()
{
	blackScreen->render();
}

void Level::resetGuards()
{
	for (Guard* guard : guards)
	{
		guard->clearBullets();
		guard->reset();
		delete guard;
	}
	guards.clear();
}

void Level::addRoller(const glm::vec2& pos, ShaderProgram& shaderProgram, bool moveRight)
{
	Roller* roller = new Roller();
	roller->init(glm::vec2(0, 0), shaderProgram, moveRight, map->getTileSize()); // sin desplazamiento
	roller->setPosition(pos);
	rollers.push_back(roller);
}

void Level::updateRollers(int deltaTime, Player* player)
{
	for (Roller* r : rollers)
		r->update(deltaTime, map, player);
}

void Level::renderRollers()
{
	for (Roller* r : rollers)
		r->render();
}

void Level::clearRollers()
{
	for (Roller* r : rollers) delete r;
	rollers.clear();
}

void Level::addArnoldBoss(const glm::vec2& pos, ShaderProgram& shaderProgram)
{
	arnoldBoss = new ArnoldBoss();
	arnoldBoss->init(shaderProgram); 
	arnoldBoss->setShaderProgram(&shaderProgram);
	arnoldBoss->setPosition(pos);

	std::cout << "[Level] ArnoldBoss creado correctamente en posición "
		<< pos.x << ", " << pos.y << std::endl;
}


void Level::addTwin(const glm::vec2& position, ShaderProgram& program)
{
	twin = new Twin();
	twin->init(program);
	twin->setShaderProgram(&program);
	twin->setPosition(position);
}


void Level::resetTwin()
{
	twin->clearBullets();
	twin->reset();
	delete twin;
	twin = nullptr;
}




