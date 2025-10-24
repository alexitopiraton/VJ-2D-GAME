#ifndef _CHARACTERS_INCLUDE
#define _CHARACTERS_INCLUDE

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Sprite.h"
#include <map>

struct CharactersInText {
	string text;
	glm::vec2 position;
	bool display;
};

class Characters
{
public:
	Characters();
	void render();
	void init(ShaderProgram& program);

	void initialise_latin_characters(ShaderProgram& program);
	void initialise_numbers_characters(ShaderProgram& program);
	void initialise_punctuation_characters(ShaderProgram& program);

	void addText(const string& id, const string& text, const glm::vec2& pos);
	void updateText(const string& id, const string& newText);
	void updatePosition(const string& id, const glm::vec2& newPos);
	void showText(const string& id);
	void hideText(const string& id);

private:
	void renderText(const string& text, const glm::vec2& position);

private:
	std::vector<Sprite*> latin;
	std::vector<Sprite*> numbers;
	std::vector<Sprite*> punctuation;

	map<string, CharactersInText> texts;
	float spacing;

};

#endif