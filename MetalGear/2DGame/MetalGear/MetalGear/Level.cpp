#include "Level.h"
#include "Guard.h"      
#include "Player.h"     
#include "Roller.h"     
#include "ArnoldBoss.h" 
#include "Twin.h"       


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
	// Liberar guardias
	for (Guard* guard : guards)
		delete guard;
	guards.clear();
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

		// Renderizar guardias
		for (Guard* guard : guards)
			guard->render();

		renderRollers();
		if (arnoldBoss)
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

void Level::update(int deltaTime, Player* player)
{
	updateRollers(deltaTime, player);

	if (arnoldBoss)
		arnoldBoss->update(deltaTime, *map, *player);

	// === ACTUALIZAR TWIN Y SUS BALAS ===
	if (twin) {
		twin->AIControl(*map, *player, deltaTime);
		twin->update(deltaTime);
		const std::list<Bullet*>& twinBullets = twin->getBullets();

		for (Bullet* b : twinBullets)
		{
			if (b->isAlive()) {
				glm::vec2 bulletPos = b->getPosition();
				glm::ivec2 bulletSize(16, 16);
				glm::ivec2 playerSize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

				bool collisionX = bulletPos.x + bulletSize.x >= player->getPosition().x &&
					player->getPosition().x + playerSize.x >= bulletPos.x;
				bool collisionY = bulletPos.y + bulletSize.y >= player->getPosition().y &&
					player->getPosition().y + playerSize.y >= bulletPos.y;

				if (collisionX && collisionY)
				{
					player->takeDamage(20);
					b->setAlive(false);
				}
			}
		}
	}

	// === COLISIONES DE BALAS DEL JUGADOR CON ENEMIGOS ===
	std::list<Bullet*>& bulletsPlayer = player->getBullets();

	for (Bullet* b : bulletsPlayer)
	{
		if (!b->isAlive()) continue;

		glm::vec2 bulletPos = b->getPosition();
		glm::ivec2 bulletSize(16, 16);
		glm::ivec2 enemySize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

		bool bulletHit = false; // Flag para saber si la bala ya impactó

		// 1. Verificar colisión con Twin
		if (twin != nullptr && !bulletHit)
		{
			glm::vec2 twinPos = twin->getPosition();
			bool collisionX = bulletPos.x + bulletSize.x >= twinPos.x &&
				twinPos.x + enemySize.x >= bulletPos.x;
			bool collisionY = bulletPos.y + bulletSize.y >= twinPos.y &&
				twinPos.y + enemySize.y >= bulletPos.y;

			if (collisionX && collisionY)
			{
				twin->takeDamage(player->getWeaponDamage());
				b->setAlive(false);
				bulletHit = true;
			}
		}

		// 2. Verificar colisión con ArnoldBoss
		if (arnoldBoss != nullptr && !bulletHit)
		{
			glm::vec2 arnoldPos = arnoldBoss->getPosition();
			bool collisionX = bulletPos.x + bulletSize.x >= arnoldPos.x &&
				arnoldPos.x + enemySize.x >= bulletPos.x;
			bool collisionY = bulletPos.y + bulletSize.y >= arnoldPos.y &&
				arnoldPos.y + enemySize.y >= bulletPos.y;

			if (collisionX && collisionY)
			{
				arnoldBoss->takeDamage(player->getWeaponDamage());
				b->setAlive(false);
				bulletHit = true;
			}
		}

		// 3. Verificar colisión con TODOS los Guards
		if (!bulletHit)
		{
			for (Guard* g : guards)
			{
				if (g == nullptr) continue;

				glm::vec2 guardPos = g->getPosition();
				bool collisionX = bulletPos.x + bulletSize.x >= guardPos.x &&
					guardPos.x + enemySize.x >= bulletPos.x;
				bool collisionY = bulletPos.y + bulletSize.y >= guardPos.y &&
					guardPos.y + enemySize.y >= bulletPos.y;

				if (collisionX && collisionY)
				{
					g->takeDamage(player->getWeaponDamage());
					b->setAlive(false);
					bulletHit = true;
					break; // Salir del loop de guards, ya impactó
				}
			}
		}
	}

	// === ACTUALIZAR GUARDIAS Y SUS BALAS ===
	for (Guard* guard : guards)
	{
		guard->AIControl(*map, *player, deltaTime);
		guard->update(deltaTime);

		const std::list<Bullet*>& guardBullets = guard->getBullets();

		for (Bullet* b : guardBullets)
		{
			if (b->isAlive()) {
				glm::vec2 bulletPos = b->getPosition();
				glm::ivec2 bulletSize(16, 16);
				glm::ivec2 playerSize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

				bool collisionX = bulletPos.x + bulletSize.x >= player->getPosition().x &&
					player->getPosition().x + playerSize.x >= bulletPos.x;
				bool collisionY = bulletPos.y + bulletSize.y >= player->getPosition().y &&
					player->getPosition().y + playerSize.y >= bulletPos.y;

				if (collisionX && collisionY)
				{
					player->takeDamage(20);
					b->setAlive(false);
				}
			}
		}
	}
}

void Level::addGuard(const glm::vec2& position, ShaderProgram& program)
{
	Guard* guard = new Guard();
	guard->init(program);
	guard->setShaderProgram(&program);
	guard->setPosition(position);
	guards.push_back(guard);
}

void Level::resetEnemies()
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
	roller->init(glm::vec2(0, 0), shaderProgram, moveRight, map->getTileSize());
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
}

void Level::addTwin(const glm::vec2& position, ShaderProgram& program)
{
	twin = new Twin();
	twin->init(program);
	twin->setShaderProgram(&program);
	twin->setPosition(position);
}

void Level::reset()
{
	for (Guard* guard : guards) {
		guard->clearBullets();
		guard->reset();
	}

	for (Roller* r : rollers) {
		r->reset();
	}

	if (twin) {
		twin->clearBullets();
		twin->reset();
	}

	// 4. Resetear Arnold Boss
	if (arnoldBoss) {
		arnoldBoss->reset();
	}

	// 6. Resetear puertas
	if (id == 4 || id == 6 || id == 11) {
		doorOpened = false;
		map->setDoorOpen(false);
	}
	else {
		doorOpened = true;
		map->setDoorOpen(true);
	}

}

bool Level::arnoldIsDead() {
	if (arnoldBoss != NULL)
		return arnoldBoss->getIsDead();
}