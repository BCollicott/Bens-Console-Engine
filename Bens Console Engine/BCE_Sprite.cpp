#include <iostream>
#include <Windows.h>
#include "BCE_Sprite.h"

BCE_Sprite::BCE_Sprite()
{
	BCE_Sprite(NULL, { 0, 0 });
}

BCE_Sprite::BCE_Sprite(CHAR_INFO* string, COORD size)
{
	BCE_Sprite::string = string;
	BCE_Sprite::size = size;
}

BCE_Sprite::BCE_Sprite(char* string, WORD attributes, COORD size)
{
	BCE_Sprite::size = size;
	BCE_Sprite::string = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * size.X * size.Y);	//  Allocate memory for sprite containing padded string
	setText(string, attributes);
}

// Sets this sprite's string to a null-terminated string of normal ASCII characters w/ common attributes
void BCE_Sprite::setText(char* string, WORD attributes)
{
	bool padding = false;	// True when null terminator has been passed, and remaining size is padded w/ spaces

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
			BCE_Sprite::string[i] = { ' ', 0 };
		}
	}
} 