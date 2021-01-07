#include "BCGL_Mask.h"

BCGL_Mask::BCGL_Mask(COORD size, bool init)
{
	BCGL_Mask::size = size;

	// Allocate memory for proper number of bytes needed to store all bits
	short numBytes = (size.X * size.Y) / sizeof(char);
	if (size.X * size.Y % 8 > 0)
	{
		numBytes++;
	}
	byteArray = (char*)malloc(numBytes * sizeof(char));

	// Set all bits to initial value
	for (short b = 0; b < numBytes; b++)
	{
		if (init)
		{
			byteArray[b] = 0b11111111;
		}
		else
		{
			byteArray[b] = 0b00000000;
		}
		
	}
}

void BCGL_Mask::freeMemory()
{
	if (byteArray != nullptr)
	{
		free(byteArray);
	}
}

COORD BCGL_Mask::getSize()
{
	return size;
}

bool BCGL_Mask::getBit(COORD maskCoord)
{
	int maskIndex = (maskCoord.X % size.X) + (maskCoord.Y * size.X);	// Index of mask bit in mask byte array
	return byteArray[maskIndex / 8] & BIT_MASK[maskIndex % 8];
}

void BCGL_Mask::setBit(COORD maskCoord, bool val)
{
	int maskIndex = (maskCoord.X % size.X) + (maskCoord.Y * size.X);	// Index of mask bit in mask byte array

	// Set bit to 0
	byteArray[maskIndex / 8] &= ~BIT_MASK[maskIndex % 8];

	// Set bit to 1 if requested
	if (val)
	{
		byteArray[maskIndex / 8] |= BIT_MASK[maskIndex % 8];
	}
}