#pragma once
#include <Windows.h>
#include "BCE_Sprite.h"

class BCE_GameObject {
	public:
		COORD pos;			// Position of GameObject in space
		BCE_Sprite* sprite;	// 2D char "sprite" to represent this GameObject
		BCE_GameObject(COORD pos, BCE_Sprite* sprite);
};