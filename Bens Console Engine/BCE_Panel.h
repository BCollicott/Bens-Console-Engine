#pragma once
#include <Windows.h>

// Rectangular area on window displaying a character array
class BCE_Panel {
	public: 
		SMALL_RECT region;	// Rectangular area/region of panel, exclusive coordinates. Size of array and centered if it is less than panel size
		COORD panelSize;	// Size of panel itself in rows/cols
		CHAR_INFO* array;	// Array of characters in the panel
		COORD arraySize;	// Size of array in rows/cols
		COORD viewportPos;	// Top left position of area of array shown by panel when array size is larger than panel

		BCE_Panel(SMALL_RECT region, COORD arraySize);	// Constructor
};