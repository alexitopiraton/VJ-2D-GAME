#include "Characters.h"

Characters::Characters()
{
	characters = "";
}

void Characters::init(ShaderProgram& program)
{
	initialise_latin_characters(program);
	initialise_numbers_characters(program);
	initialise_punctuation_characters(program);

	display = false;
	spacing = 20.f;
	initialPos = glm::vec2(20, 440);
	actualPos = initialPos;
}

void Characters::initialise_latin_characters(ShaderProgram& program)
{
	string latin_characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < latin_characters.size(); i++)
	{
		stringstream ss;
		ss << "images/Characters/Latin/" << latin_characters[i] << ".png";
		string filename = ss.str();

		Texture* texture = new Texture();
		texture->loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);

		Sprite* sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(1.f, 1.f), texture, &program);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 1);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->changeAnimation(0);

		latin.push_back(sprite);
	}
}

void Characters::initialise_numbers_characters(ShaderProgram& program)
{
	string numbers_characters = "0123456789";

	for (int i = 0; i < numbers_characters.size(); i++)
	{
		stringstream ss;
		ss << "images/Characters/Numbers/" << numbers_characters[i] << ".png";
		string filename = ss.str();

		Texture* texture = new Texture();
		texture->loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);

		Sprite* sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(1.f, 1.f), texture, &program);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 1);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->changeAnimation(0);

		numbers.push_back(sprite);
	}
}

void Characters::initialise_punctuation_characters(ShaderProgram& program)
{
	string punctuation_characters = ".";

	for (int i = 0; i < punctuation_characters.size(); i++)
	{
		stringstream ss;
		ss << "images/Characters/Punctuation/" << punctuation_characters[i] << ".png";
		string filename = ss.str();

		Texture* texture = new Texture();
		texture->loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);

		Sprite* sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(1.f, 1.f), texture, &program);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 1);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->changeAnimation(0);

		punctuation.push_back(sprite);
	}
}

void Characters::render()
{
	if (display)
	{
		actualPos = initialPos;
		for (int i = 0; i < characters.size(); i++)
		{
			char c = characters[i];
			int positionInVector;
			Sprite* sprite;

			if (c == ' ')
			{
				actualPos.x += spacing;
				continue;
			}
			else if (c >= 'A' && c <= 'Z')
			{
				positionInVector = c - 'A';
				sprite = latin[positionInVector];
			}
			else if (c >= 'a' && c <= 'z')
			{
				positionInVector = c - 'a';
				sprite = latin[positionInVector];
			}
			else if (c >= '0' && c <= '9')
			{
				positionInVector = c - '0';
				sprite = numbers[positionInVector];
			}
			else
			{
				positionInVector = 0;
				sprite = punctuation[positionInVector];
			}

			sprite->setPosition(actualPos);
			sprite->render();
			actualPos.x += spacing;
		}
	}
}

void Characters::setCharacters(const string& newCharacters)
{
	characters = newCharacters;
	display = true;
}

void Characters::stopDisplay()
{
	display = false;
}