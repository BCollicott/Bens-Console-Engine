#pragma once
#include <Windows.h>

class BCE_GameObject {
	public:
		int x;
		int y;
		CHAR_INFO* character;	// Character used to represent this GameObject
		BCE_GameObject(int x, int y, CHAR_INFO* character);
};