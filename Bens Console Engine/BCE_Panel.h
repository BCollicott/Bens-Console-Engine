#pragma once
#include <Windows.h>
#include "BCE_Space.h"

// Rectangular area on window displaying a character array
class BCE_Panel {
	public: 
		SMALL_RECT region;	// Rectangular area/region of panel, exclusive coordinates. Size of array and centered if it is less than panel size
		COORD panelSize;	// Size of panel itself in rows/cols
		BCE_Space* space;	// Space viewed by this panel
		COORD viewportPos;	// Top left position of area of array shown by panel when array size is larger than panel

		BCE_Panel(SMALL_RECT region);	// Constructor
		CHAR_INFO* getContents();		// Get all chars of space that are visible in this panel to be drawn to buffer
};