#pragma once
#include <Windows.h>

class BCE_Mask {
	COORD size;			// Dimensions of GameObject in rows/cols
	char* byteArray;	// Bitwise mask for sprite drawing and collisions

	// Masks for individual bytes of byte array, used to access individual bit according to index
	// Allows for guarenteed constant time bit setting/getting by avoiding multiple bit shifts
	const char BIT_MASK[8] = { 
		(char)0b00000001, 
		(char)0b00000010, 
		(char)0b00000100, 
		(char)0b00001000, 
		(char)0b00010000, 
		(char)0b00100000, 
		(char)0b01000000, 
		(char)0b10000000 };

public:
	BCE_Mask(COORD size, bool init);		// Constructor

	void freeMemory();						// Frees memory allocated by mask

	COORD getSize();						// Get size of mask
	bool getBit(COORD maskCoord);			// Getvalue of bit from mask

	void setBit(COORD maskCoord, bool val);	// Set value of bit in mask
};