#pragma once
#include <Windows.h>

class BCE_Sprite {
public:
	CHAR_INFO* string;	// String of characters representing sprite
	COORD size;			// Number of rows/cols to divide string into

	BCE_Sprite();
	BCE_Sprite(COORD size);
	BCE_Sprite(CHAR_INFO* string, COORD size);
	BCE_Sprite(char* string, WORD attributes, COORD size);

	void setText(char* string, WORD attributes, WCHAR padChar);	// Set sprite to a left-justified null-terminated string with common attributes
	void setValue(int value, WORD attributes, WCHAR padChar);	// Set sprite to a right-justified integer with common attributes

private:
	void init(CHAR_INFO* string, COORD size);		// Shared code for overloaded constructors to set size and string
};

char* shortToChar(SHORT val);	// Convert a short to an ASCII string