#pragma once
#include <Windows.h>
#include "BCGL_Sprite.h"
#include "BCGL_Mask.h"

// Types of collisions
#define COLLIDER_NONE 0x00	// GameObject has no collisions
#define COLLIDER_RECT 0x01	// Collisions determined by pos + size
#define COLLIDER_MASK 0x02	// Collisions determines by 1 bits on mask

class BCGL_GameObject {
	COORD pos;			// Position of GameObject in space
	COORD size;			// Dimensions of GameObject in rows/cols
	BCGL_Sprite* sprite;	// 2D char "sprite" to represent this GameObject
	BCGL_Mask* mask;		// Bitwise mask for sprite drawing and collisions
	char colliderType;	// Macro containing type of collision

	void init(COORD pos, COORD size, BCGL_Sprite* sprite);	// Shared code for overloaded constructors to set pos, size, sprite, and collider type

	public:
		BCGL_GameObject(COORD pos);
		BCGL_GameObject(COORD pos, BCGL_Sprite* sprite);
		BCGL_GameObject(COORD pos, COORD size, BCGL_Sprite* sprite);

		void transate(COORD delta);
		COORD objectCoordToSpaceCoord(COORD objectCoord);	// Get the coordinates in space of a y-down coordinate relative to this object's origin
		COORD spaceCoordToObjectCoord(COORD spaceCoord);	// Get the y-down coordianate relative to this object's origin of a position in space
		
		COORD getPos();
		COORD getSize();
		BCGL_Sprite* getSprite();
		BCGL_Mask* getMask();
		char getColliderType();

		void setPos(COORD pos);
		void setSize(COORD size);
		void setSprite(BCGL_Sprite* sprite);
		void setMask(BCGL_Mask* mask);
		void setColliderType(char colliderType);
};