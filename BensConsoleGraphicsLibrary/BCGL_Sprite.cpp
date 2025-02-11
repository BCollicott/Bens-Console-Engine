#include <iostream>
#include <fstream>
#include <Windows.h>
#include "BCGL_Sprite.h"

using namespace std;

BCGL_Sprite::BCGL_Sprite()
{
	init(NULL, { 0, 0 });
}

BCGL_Sprite::BCGL_Sprite(COORD size)
{
	// Allocate memory for string based on size
	init((BCGL_Char*)malloc(sizeof(BCGL_Char) * size.X * size.Y), size);
}

BCGL_Sprite::BCGL_Sprite(BCGL_Char* string, COORD size)
{
	init(string, size);
}

BCGL_Sprite::BCGL_Sprite(char* string, unsigned char color, COORD size)
{
	// Allocate memory for string based on size
	init((BCGL_Char*)malloc(sizeof(BCGL_Char) * size.X * size.Y), size);

	// Set text within allocated string memory
	setText(string, color, { ' ', 0 });
}

void BCGL_Sprite::init(BCGL_Char* string, COORD size)
{
	BCGL_Sprite::size = size;
	BCGL_Sprite::string = string;
}

void BCGL_Sprite::freeMemory()
{
	free(string);
}

// Serialize sprite to a file containing size and contents of string
// @param path Name and path of file to be created or overwritten
// @return True if file was successfully opened/created and written to
bool BCGL_Sprite::serialize(const char* path)
{
	ofstream spriteFile(path, ios::trunc | ios::binary);	// Binary output file stream, creates new file or deletes existing contents

	if (spriteFile.is_open())
	{
		// Write X and Y dimensions
		spriteFile.write((char*)&size, sizeof(COORD));

		// Write contents of string
		spriteFile.write((char*)string, ((short)sizeof(BCGL_Char)) * size.X * size.Y);

		spriteFile.close();

		return true;
	}

	return false;
}

// Deserialize/read sprite data from a file and create a new sprite object using it
// @param path Name and path of file used to create new sprite
// @return Pointer to new sprite, or nullptr if file could not be opened
BCGL_Sprite* BCGL_Sprite::deserialize(const char* path)
{
	ifstream spriteFile(path, ios::binary);	// Binary input files stream
	BCGL_Sprite* newSprite = nullptr;		// Sprite being created from file

	if (spriteFile.is_open())
	{

		// Read X and Y dimensions to temporary COORD struct
		COORD size = { 0, 0 };
		spriteFile.read((char*)&size, sizeof(COORD));

		// Create sprite object to allocate correct amount of string memory
		newSprite = new BCGL_Sprite(size);

		// Read contents of string to new object
		spriteFile.read((char*)newSprite->getString(), ((short)sizeof(BCGL_Char)) * size.X * size.Y);

		spriteFile.close();
	}

	return newSprite;
}

BCGL_Char* BCGL_Sprite::getString()
{
	return string;
}

BCGL_Char BCGL_Sprite::getCharacter(COORD spriteCoord)
{
	return string[spriteCoord.X % size.X + (spriteCoord.Y % size.Y) * size.X];
}

void BCGL_Sprite::setCharacter(COORD spriteCoord, BCGL_Char chraracter)
{
	string[spriteCoord.X % size.X + (spriteCoord.Y % size.Y) * size.X] = chraracter;
}

// Sets this sprite's string to a null-terminated string of normal ASCII characters w/ common color
void BCGL_Sprite::setText(char* string, unsigned char color, BCGL_Char padChar)
{
	bool padding = false;	// True when null terminator has been passed, and remaining size is padded

	for (int i = 0; i < size.X * size.Y; i++)
	{
		// Begin padding string if null terminator is reached
		if (string[i] == 0)
		{
			padding = true;
		}
		
		// Add char from text to sprite, or space if padding
		if (!padding)
		{
			BCGL_Sprite::string[i].code = string[i];
			BCGL_Sprite::string[i].color = color;
		}
		else
		{
			BCGL_Sprite::string[i] = padChar;
		}
	}
} 

void BCGL_Sprite::setValue(int value, unsigned char color, BCGL_Char padChar)
{
	bool padding = false;	// True when all digits have been read, and remaining size is padded

	// Add ASCII digits to string from least significant on right to msot significant on left
	for (int c = size.X - 1; c >= 0; c--)
	{

		// Get next least significant bit of value and add it to the next rightmost position in the string
		// Do once before checking if padding is necessary so value of 0 will still be displayed even is padChar is not '0'
		if (!padding)
		{
			BCGL_Sprite::string[c].code = value % 10 + '0';
			BCGL_Sprite::string[c].color = color;
			value /= 10;
		}
		else
		{
			BCGL_Sprite::string[c] = padChar;
		}

		if (value == 0) {
			padding = true;
		}
	}
}

COORD BCGL_Sprite::getSize()
{
	return size;
}
