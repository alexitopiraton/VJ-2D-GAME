#ifndef _MAP_INCLUDE
#define _MAP_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class Map {
private:
	Map();

public:
	static Map *createMap();
	void render(const Texture& tex) const;

	bool collisionMoveLeft();
	bool collisionMoveRight();
	bool collisionMoveDown();
	bool collisionMoveUp();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;


};

#endif