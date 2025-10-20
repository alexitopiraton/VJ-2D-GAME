#ifndef _CHARACTERS_INCLUDE
#define _CHARACTERS_INCLUDE

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Sprite.h"

class Characters
{
public:
	Characters();
	void render();
	void init(ShaderProgram& program);

	void initialise_latin_characters(ShaderProgram& program);
	void initialise_numbers_characters(ShaderProgram& program);
	void initialise_punctuation_characters(ShaderProgram& program);

	void setCharacters(const string& newCharacters);
	void stopDisplay();

private:
	std::vector<Sprite*> latin;
	std::vector<Sprite*> numbers;
	std::vector<Sprite*> punctuation;

	string characters;
	float spacing;
	bool display;
	glm::vec2 initialPos;
	glm::vec2 actualPos;
};

#endif