#pragma once
#include <thread>
#include <vector>
#include <iostream>
#include <Windows.h>

// Bit flags bitwise OR'd together in modifierFlags field of BCGL_KeyStroke
#define MF_SHIFT 0x01
#define MF_CTRL 0x02
#define MF_ALT 0x03

using namespace std;

struct BCGL_KeyPress {
	char keyCode;
	bool isPressed;
};

struct BCGL_KeyStroke {
	char keyCode;
	char modifierFlags;
	bool isPressed;
};

// Contains collection of Key presses, Axes, and Keystrokes checked in a thread, which can be retrieved by main program asynchronously
class BCGL_InputManager {
	thread* inputThread;	// Pointer to thread running this input manager
	bool newFrame;			// True if main thread has requested inputs for new frame
	bool running;			// True if inputThread should continue running
	vector<BCGL_KeyPress*> keyPresses;
	vector<BCGL_KeyStroke*> keyStrokes;

	short keyMask;   // Mask containing most sigificant byte of short for reading key input from getKeyState

	public:
		BCGL_InputManager();

		BCGL_KeyPress* addKeyPress(char keyCode);
		BCGL_KeyStroke* addKeyStroke(char keyCode, char modifierFlags);
		void detectInput();	// Get used input and indicate button presses, etc. Which occured since last frame in main thread
		void start();		// Begin thread/check for inputs
		void nextFrame();	// Used by main thread to indicate it has processed frame, and is ready for the next
		void stop();		// Safely end/join thread

		bool isNewFrame();
		bool isRunning();
};

void inputManagerThread(BCGL_InputManager* inputManager);	// Function executed in seperate thread for input manager

/*
class BCGL_KeyPress {
	char keyCode;	// Windows virtual keycode being detected
	bool pressed;

	public:
		BCGL_KeyPress(char keyCode);

		bool isPressed();
};*/