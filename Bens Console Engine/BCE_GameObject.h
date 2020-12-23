#pragma once
#include <Windows.h>
#include "BCE_Sprite.h"

// Types of collisions
#define COLLIDER_NONE 0x00	// GameObject has no collisions
#define COLLIDER_RECT 0x01	// Collisions determined by pos + size
#define COLLIDER_MASK 0x02	// Collisions determines by 1 bits on mask

class BCE_GameObject {
	COORD pos;			// Position of GameObject in space
	COORD size;			// Dimensions of GameObject in rows/cols
	BCE_Sprite* sprite;	// 2D char "sprite" to represent this GameObject
	BYTE* mask;			// Bitwise mask for sprite drawing and collisions
	BYTE colliderType;	// Macro containing type of collision

	void init(COORD pos, COORD size, BCE_Sprite* sprite);	// Shared code for overloaded constructors to set pos, size, sprite, and collider type

	public:
		BCE_GameObject(COORD pos);
		BCE_GameObject(COORD pos, BCE_Sprite* sprite);
		BCE_GameObject(COORD pos, COORD size, BCE_Sprite* sprite);

		void transate(COORD delta);
		void addMask(bool init);
		void freeMask();
		COORD objectCoordToSpaceCoord(COORD objectCoord);	// Get the coordinates in space of a y-down coordinate relative to this object's origin
		COORD spaceCoordToObjectCoord(COORD spaceCoord);	// Get the y-down coordianate relative to this object's origin of a position in space
		
		COORD getPos();
		COORD getSize();
		BCE_Sprite* getSprite();
		BOOL getMaskBit(COORD maskCoord);
		BYTE getColliderType();

		void setPos(COORD pos);
		void setSize(COORD size);
		void setSprite(BCE_Sprite* sprite);
		void setMaskBit(SHORT col, SHORT row, bool val);
		void setColliderType(BYTE colliderType);
};