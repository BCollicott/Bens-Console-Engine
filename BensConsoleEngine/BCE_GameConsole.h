#pragma once
#include <Windows.h>
#include <vector>
#include "BCE_Panel.h"

// Fullscreen console of variable dimension for displaying text within multiple panels. 
class BCE_GameConsole {
	HANDLE consoleBuffer;			// Windows handle to console screen buffer
	COORD consoleSize;				// Dimensions of buffer/window in characters
	std::vector<BCE_Panel*> panels;	// Pointers to panels contained in this console

	public:
		BCE_GameConsole(short width, short height);				// Constructor

		bool show(bool stretch);										// Attempt to create buffer and display on screen
		bool addPanel(BCE_Panel* panel);								// Add panel to this console
		bool drawPanelBorder(BCE_Panel* panel, CHAR_INFO* borderChar);	// Draw a border around a panel
		bool updateConsoleBuffer();										// Update console buffer
		
};
