#pragma once
#include <Windows.h>
#include <vector>
#include "BCGL_Panel.h"

// Fullscreen console of variable dimension for displaying text within multiple panels. 
class BCGL_GameConsole {
	HANDLE buffer;			// Windows handle to console screen buffer
	COORD consoleSize;				// Dimensions of buffer/window in characters
	std::vector<BCGL_Panel*> panels;	// Pointers to panels contained in this console

	public:
		BCGL_GameConsole(short width, short height);				// Constructor

		bool show(bool stretch);										// Attempt to create buffer and display on screen
		bool addPanel(BCGL_Panel* panel);								// Add panel to this console
		bool drawPanelBorder(BCGL_Panel* panel, CHAR_INFO* borderChar);	// Draw a border around a panel
		bool updateBuffer();										// Update console buffer
		
};
