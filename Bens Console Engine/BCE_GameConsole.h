#pragma once
#include <Windows.h>
#include <vector>
#include "BCE_Panel.h"

// Console window containing several panels
class BCE_GameConsole {
	HANDLE consoleBuffer;			// Windows buffer for console
	COORD consoleSize;				// Dimensions of buffer/window in characters
	std::vector<BCE_Panel*> panels;	// Vector of pointers to panels contained in this console

	public:
		BCE_GameConsole(short width, short height);				// Constructor
		bool show(bool stretchToFit);							// Attempt to create buffer and display on screen
		bool addPanel(BCE_Panel* panel, CHAR_INFO* borderChar);	// Add panel to this console w/ border if not null
		bool updateConsoleBuffer();								// Update console buffer
		
};
