#pragma once
#include <Windows.h>

class BCE_Sprite {
	CHAR_INFO* string;	// String of characters representing sprite, unique to this sprite
	COORD size;			// Number of rows/cols to divide string into

	void init(CHAR_INFO* string, COORD size);		// Shared code for overloaded constructors to set size and string

public:
	BCE_Sprite();
	BCE_Sprite(COORD size);
	BCE_Sprite(CHAR_INFO* string, COORD size);
	BCE_Sprite(char* string, WORD attributes, COORD size);

	void freeMemory();									// Free memory allocated within this object
	bool serialize(const char* path);					// Serialize this sprite to a file
	static BCE_Sprite* deserialize(const char* path);	// Create a sprite object from a serialized sprite file

	CHAR_INFO* getString();
	CHAR_INFO getCharacter(COORD spriteCoord);

	void setCharacter(COORD spriteCoord, CHAR_INFO chraracter);
	void setText(char* string, WORD attributes, WCHAR padChar);	// Set sprite to a left-justified null-terminated string with common attributes
	void setValue(int value, WORD attributes, WCHAR padChar);	// Set sprite to a right-justified integer with common attributes
	
	COORD getSize();
};