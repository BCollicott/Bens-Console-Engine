#pragma once
#include <Windows.h>
#include "BCE_Space.h"

// Rectangular area on window displaying a character array
class BCE_Panel {
	SMALL_RECT writeRegion;	// Rectangular region of the console that panel is drawn to
	COORD panelSize;		// Size of panel itself in rows/cols
	BCE_Space* space;		// Space viewed by this panel
	COORD posInSpace;		// Position of the top left corner of the area displayed by panel in the space

	public:
		BCE_Panel(BCE_Space* space, SMALL_RECT writeRegion);	// Constructor

		CHAR_INFO* getSpaceArray();		// Get array of chars from GameObjects of space that are visible in this panel to be drawn to buffer
		SMALL_RECT getWriteRegion();	// Get region that panel is drawn to in buffer
		COORD getPanelSize();			// Get size of write region/panel 

		void setSpace(BCE_Space* space);				// Set space to be displayed within this panel
		void setPosInSpace(COORD posInSpace);			// Set panel's position in space
		void setWriteRegion(SMALL_RECT writeRegion);	// Set region panel is written to in the buffer and recalcualte panel size
};