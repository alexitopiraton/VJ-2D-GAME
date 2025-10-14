#include "Level.h"
#include <iostream>

Level::Level()
{
	map = NULL;
	background = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (background != NULL)
		delete background;

	// Liberar guardias
	for (Guard* guard : guards)
		delete guard;
	guards.clear();
}

TileMap* Level::get_tile_map()
{
	return map;
}

void Level::init(const string& levelFile, const string& backgroundFile, const glm::vec2& minCoords, ShaderProgram& program, const bool& outside, const glm::vec2& positionInSpritesheet)
{
	map = TileMap::createTileMap(levelFile, minCoords, program);

	backgroundImage.loadFromFile(backgroundFile, TEXTURE_PIXEL_FORMAT_RGBA);
	if(outside)
		background = Sprite::createSprite(glm::ivec2(640,480), glm::vec2(0.5f, IMAGE_OFFSET), &backgroundImage, &program);
	else
		background = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(IMAGE_OFFSET, IMAGE_OFFSET), &backgroundImage, &program);

	background->setNumberAnimations(1);
	background->setAnimationSpeed(0, 1);
	background->addKeyframe(0, positionInSpritesheet);
	background->changeAnimation(0);
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
	cout << "[Level] Updating level with ArnoldBoss: " << (arnoldBoss ? "Yes" : "No") << endl;
	if(arnoldBoss)
		arnoldBoss->update(deltaTime, *map, *player);

	cout << "[Level] ArnoldBoss updated." << endl;

	// Actualizar todos los guardias
	for (Guard* guard : guards)
	{
		cout << "Updating guard at position: " << endl;
		guard->AIControl(*map, *player, deltaTime);
		guard->update(deltaTime);

		// --- Comprobar colisiones de balas del guardia con el jugador ---
		const std::list<Bullet*>& bullets = guard->getBullets();



		for (Bullet* b : bullets)
		{
			std::cout << "[Level] :" << b->isAlive()<< std::endl;
			std::cout << "[Level] :" << b->isAlive() << std::endl;
			std::cout << "[Level] :" << b->isAlive() << std::endl;
			std::cout << "[Level] :" << b->isAlive() << std::endl;
			std::cout << "[Level] :" << b->isAlive() << std::endl;
			std::cout << "[Level] :" << b->isAlive() << std::endl;
			std::cout << "[Level] :" << b->isAlive() << std::endl;
			// Si la bala está viva
			if (b->isAlive()) {

				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				std::cout << "[Level] ¡Tratando bala!" << std::endl;
				glm::vec2 bulletPos = b->getPosition();
				glm::ivec2 bulletSize(16, 16);
				glm::ivec2 playerSize(SPRITE_WIDTH + 10, SPRITE_HEIGHT + 10);

				bool collisionX = bulletPos.x + bulletSize.x >= player->getPosition().x &&
					player->getPosition().x + playerSize.x >= bulletPos.x;
				bool collisionY = bulletPos.y + bulletSize.y >= player->getPosition().y &&
					player->getPosition().y + playerSize.y >= bulletPos.y;

				if (collisionX && collisionY) // radio de colisión
				{
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					std::cout << "[Level] ¡Bala impactó al jugador!" << std::endl;
					player->takeDamage(20);
					b->setAlive(false);
				}
			}
		}
	}
}

void Level::render()
{
	map->render();
	background->render();

	// Renderizar guardias
	for (Guard* guard : guards)
		guard->render();

	renderRollers();
	if(arnoldBoss)
		arnoldBoss->render();
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
	roller->init(glm::vec2(0, 0), shaderProgram, moveRight); // sin desplazamiento
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





