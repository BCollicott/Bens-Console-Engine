#pragma once
#include <Windows.h>
#include "BCE_Sprite.h"

class BCE_GameObject {
	BCE_Sprite* sprite;	// 2D char "sprite" to represent this GameObject

	public:
		COORD pos;			// Position of GameObject in space
		COORD size;			// Dimensions of GameObject in rows/cols
		
		BCE_GameObject(COORD pos, COORD size, BCE_Sprite* sprite);
		BCE_GameObject(COORD pos, BCE_Sprite* sprite);

		BCE_Sprite* getSprite();
};