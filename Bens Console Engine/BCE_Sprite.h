#pragma once
#include <Windows.h>

class BCE_Sprite {
public:
	CHAR_INFO* string;	// String of characters representing sprite
	COORD size;			// Number of rows/cols to divide string into

	BCE_Sprite();
	BCE_Sprite(CHAR_INFO* string, COORD size);
	BCE_Sprite(char* string, WORD attributes, COORD size);

	void setText(char* string, WORD attributes);	// Set sprite to a null-terminated string with common attributes
};