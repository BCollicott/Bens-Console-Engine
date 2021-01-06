#include <iostream>
#include <fstream>
#include <Windows.h>
#include "BCE_Sprite.h"

using namespace std;

BCE_Sprite::BCE_Sprite()
{
	init(NULL, { 0, 0 });
}

BCE_Sprite::BCE_Sprite(COORD size)
{
	// Allocate memory for string based on size
	init((CHAR_INFO*)malloc(sizeof(CHAR_INFO) * size.X * size.Y), size);
}

BCE_Sprite::BCE_Sprite(CHAR_INFO* string, COORD size)
{
	init(string, size);
}

BCE_Sprite::BCE_Sprite(char* string, WORD attributes, COORD size)
{
	// Allocate memory for string based on size
	init((CHAR_INFO*)malloc(sizeof(CHAR_INFO) * size.X * size.Y), size);

	// Set text within allocated string memory
	setText(string, attributes, ' ');
}

void BCE_Sprite::init(CHAR_INFO* string, COORD size)
{
	BCE_Sprite::size = size;
	BCE_Sprite::string = string;
}

void BCE_Sprite::freeMemory()
{
	free(string);
}

// Serialize sprite to a file containing size and contents of string
// @param path Name and path of file to be created or overwritten
// @return True if file was successfully opened/created and written to
bool BCE_Sprite::serialize(const char* path)
{
	ofstream spriteFile(path, ios::trunc | ios::binary);	// Binary output file stream, creates new file or deletes existing contents

	if (spriteFile.is_open())
	{
		// Write X and Y dimensions
		spriteFile.write((char*)&size, sizeof(COORD));

		// Write contents of string
		spriteFile.write((char*)string, ((short)sizeof(CHAR_INFO)) * size.X * size.Y);

		spriteFile.close();

		return true;
	}

	return false;
}

// Deserialize/read sprite data from a file and create a new sprite object using it
// @param path Name and path of file used to create new sprite
// @return Pointer to new sprite, or nullptr if file could not be opened
BCE_Sprite* BCE_Sprite::deserialize(const char* path)
{
	ifstream spriteFile(path, ios::binary);	// Binary input files stream
	BCE_Sprite* newSprite = nullptr;		// Sprite being created from file

	if (spriteFile.is_open())
	{

		// Read X and Y dimensions to temporary COORD struct
		COORD size = { 0, 0 };
		spriteFile.read((char*)&size, sizeof(COORD));

		// Create sprite object to allocate correct amount of string memory
		newSprite = new BCE_Sprite(size);

		// Read contents of string to new object
		spriteFile.read((char*)newSprite->getString(), ((short)sizeof(CHAR_INFO)) * size.X * size.Y);

		spriteFile.close();
	}

	return newSprite;
}

CHAR_INFO* BCE_Sprite::getString()
{
	return string;
}

CHAR_INFO BCE_Sprite::getCharacter(COORD spriteCoord)
{
	return string[spriteCoord.X % size.X + (spriteCoord.Y % size.Y) * size.X];
}

void BCE_Sprite::setCharacter(COORD spriteCoord, CHAR_INFO chraracter)
{
	string[spriteCoord.X % size.X + (spriteCoord.Y % size.Y) * size.X] = chraracter;
}

// Sets this sprite's string to a null-terminated string of normal ASCII characters w/ common attributes
void BCE_Sprite::setText(char* string, WORD attributes, WCHAR padChar)
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
			BCE_Sprite::string[i].Char.UnicodeChar = string[i];
			BCE_Sprite::string[i].Attributes = attributes;
		}
		else
		{
			BCE_Sprite::string[i] = { padChar, attributes };
		}
	}
} 

void BCE_Sprite::setValue(int value, WORD attributes, WCHAR padChar)
{
	bool padding = false;	// True when all digits have been read, and remaining size is padded

	// Add ASCII digits to string from least significant on right to msot significant on left
	for (int c = size.X - 1; c >= 0; c--)
	{

		// Get next least significant bit of value and add it to the next rightmost position in the string
		// Do once before checking if padding is necessary so value of 0 will still be displayed even is padChar is not '0'
		if (!padding)
		{
			BCE_Sprite::string[c].Char.UnicodeChar = value % 10 + '0';
			BCE_Sprite::string[c].Attributes = attributes;
			value /= 10;
		}
		else
		{
			BCE_Sprite::string[c] = { padChar, attributes };
		}

		if (value == 0) {
			padding = true;
		}
	}
}

COORD BCE_Sprite::getSize()
{
	return size;
}
