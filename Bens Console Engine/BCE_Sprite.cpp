#include <iostream>
#include <Windows.h>
#include "BCE_Sprite.h"

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

char* shortToChar(SHORT val) {
	return NULL;
}