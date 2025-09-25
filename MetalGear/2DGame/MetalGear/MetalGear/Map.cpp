#include <iostream>
#include <fstream>
#include <sstream>
#include "Map.h"

using namespace std;



Map::Map()
{

}

Map* Map::createMap()
{
	Map *map = new Map();

	return map;
}

void Map::render(const Texture &tex) const
{
	glEnable(GL_TEXTURE_2D);
	tex.use();
	glBindVertexArray(vao);
	glDisable(GL_TEXTURE_2D);
}

bool Map::collisionMoveLeft()
{
	return false;
}

bool Map::collisionMoveRight()
{
	return false;
}

bool Map::collisionMoveDown()
{
	return false;
}

bool Map::collisionMoveUp()
{
	return false;
}