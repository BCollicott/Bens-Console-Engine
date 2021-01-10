#include <iostream>
#include "BCGL_InputManager.h"

BCGL_InputManager::BCGL_InputManager()
{
	BCGL_InputManager::inputThread = nullptr;
	BCGL_InputManager::newFrame = true;
	BCGL_InputManager::running = false;
	BCGL_InputManager::keyMask = (short)0xFF << (sizeof(short) - 1) * 8;
}

BCGL_KeyPress* BCGL_InputManager::addKeyPress(char keyCode)
{
	BCGL_KeyPress* newKeyPress = (BCGL_KeyPress*)malloc(sizeof(BCGL_KeyPress));
	newKeyPress->keyCode = keyCode;
	newKeyPress->isPressed = false;
	keyPresses.push_back(newKeyPress);
	return newKeyPress;
}

BCGL_KeyStroke* BCGL_InputManager::addKeyStroke(char keyCode, char modifierFlags)
{
	BCGL_KeyStroke* newKeyStroke = (BCGL_KeyStroke*)malloc(sizeof(BCGL_KeyStroke));
	newKeyStroke->keyCode = keyCode;
	newKeyStroke->modifierFlags = modifierFlags;
	newKeyStroke->isPressed = false;
	keyStrokes.push_back(newKeyStroke);
	return newKeyStroke;
}

void BCGL_InputManager::detectInput()
{
	// Iterate through all added key presses and determine if they have been pressed since last frame
	for (short i = 0; i < keyPresses.size(); i++)
	{
		// Reset key press if main thread has finished processing frame
		if (newFrame)
		{
			keyPresses[i]->isPressed = false;
		}

		// Indicate key has been pressed since last frame if it is currently down
		if (GetKeyState(keyPresses[i]->keyCode) & 0xFF00)
		{
			keyPresses[i]->isPressed = true;
		}
	}

	// Determine which modifier keys are currently pressed
	char activeModifierFlags = 0;
	if (GetKeyState(VK_SHIFT) & 0xFF00)
	{
		activeModifierFlags |= MF_SHIFT;
	}
	if (GetKeyState(VK_CONTROL) & 0xFF00)
	{
		activeModifierFlags |= MF_CTRL;
	}
	if (GetKeyState(VK_MENU) & 0xFF00)
	{
		activeModifierFlags |= MF_ALT;
	}

	// Iterate through all added keystrokes and determine if they have been pressed since last frame
	for (short i = 0; i < keyStrokes.size(); i++)
	{
		// Reset key press if main thread has finished processing frame
		if (newFrame)
		{
			keyStrokes[i]->isPressed = false;
		}

		// Indicate key has been pressed since last frame if it is currently down
		if ((GetKeyState(keyStrokes[i]->keyCode) & 0xFF00) && (keyStrokes[i]->modifierFlags == activeModifierFlags))
		{
			keyStrokes[i]->isPressed = true;
		}
	}

	// Reset newFrame incase it was true during iteration
	newFrame = false;
}

// Indicate input manager is running and start a new thread 
void BCGL_InputManager::start()
{
	running = true;
	inputThread = new thread(inputManagerThread, this);

}

void BCGL_InputManager::nextFrame()
{
	newFrame = true;
}

// Stop input manager from running and join input thread
void BCGL_InputManager::stop()
{
	// Indicate input manager thread should stop running
	if (running)
	{
		running = false;
	}

	// Wait until inputManagerThread has finished the final iteration of its loop and join thread
	inputThread->join();

	// Free memory
	delete(inputThread);
}

bool BCGL_InputManager::isNewFrame()
{
	return newFrame;
}

bool BCGL_InputManager::isRunning()
{
	return running;
}

void inputManagerThread(BCGL_InputManager* inputManager)
{
	while (inputManager->isRunning())
	{
		inputManager->detectInput();
	}
}

