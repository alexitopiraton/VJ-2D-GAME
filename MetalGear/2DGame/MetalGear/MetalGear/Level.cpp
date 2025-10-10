#include "Level.h"

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

void Level::render()
{
	map->render();
	background->render();
}
