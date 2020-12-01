#pragma once
#include <Windows.h>
#include <vector>
#include "BCE_Panel.h"

// Console window containing several panels
class BCE_GameConsole {
	public:
		BCE_GameConsole(short width, short height);	// Constructor
		bool show(bool stretchToFit);				// Attempt to create buffer and display on screen
		bool addPanel(BCE_Panel* panel);			// Add panel to this console
		bool update();								// Update console

	private:
		HANDLE screenBuffer;			// Windows buffer for console
		COORD consoleSize;				// Dimensions of buffer/window in characters
		std::vector<BCE_Panel*> panels;	// Vector of pointers to panels contained in this console
};
