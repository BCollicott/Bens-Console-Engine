#include <math.h>
#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

BCE_GameObject::BCE_GameObject(COORD pos)
{
	init(pos, { 0, 0 }, NULL);
}

BCE_GameObject::BCE_GameObject(COORD pos, BCE_Sprite* sprite)
{
	init(pos, sprite->getSize(), sprite);
}

BCE_GameObject::BCE_GameObject(COORD pos, COORD size, BCE_Sprite* sprite)
{
	init(pos, size, sprite);
}

void BCE_GameObject::init(COORD pos, COORD size, BCE_Sprite* sprite)
{
	BCE_GameObject::pos = pos;
	BCE_GameObject::size = size;
	BCE_GameObject::sprite = sprite;
	BCE_GameObject::mask == NULL;
	BCE_GameObject::colliderType = COLLIDER_NONE;
}

void BCE_GameObject::transate(COORD delta)
{
	pos.X += delta.X;
	pos.Y += delta.Y;
}

void BCE_GameObject::addMask(bool init)
{
	// Free old mask if it exists
	if (mask != NULL)
	{
		free(mask);
	}

	// Determine number of bytes needed to store bitwise mask
	SHORT numBytes = size.X * size.Y / 8;
	if (size.X * size.Y % 8 != 0)
	{
		numBytes++;
	}

	// Allocate memory for mask
	mask = (BYTE*)malloc(sizeof(BYTE) * numBytes);

	// Set all mask bits to initial value
	BYTE initialByte = 0;
	if (init)
	{
		initialByte = MAXBYTE;
	}
	for (int b = 0; b < numBytes; b++)
	{
		mask[b] = initialByte;
	}
}

void BCE_GameObject::freeMask()
{
	if (mask != NULL)
	{
		free(mask);
	}
}

COORD BCE_GameObject::objectCoordToSpaceCoord(COORD objectCoord)
{
	return { pos.X + objectCoord.X, pos.Y - objectCoord.Y };
}

COORD BCE_GameObject::spaceCoordToObjectCoord(COORD spaceCoord)
{
	return { spaceCoord.X - pos.X, pos.Y - spaceCoord.Y };
}

COORD BCE_GameObject::getPos()
{
	return pos;
}

COORD BCE_GameObject::getSize()
{
	return size;
}

BOOL BCE_GameObject::getMaskBit(COORD maskCoord)
{
	if (maskCoord.X < 0 || maskCoord.Y < 0 || maskCoord.X >= size.X || maskCoord.Y >= size.Y)
	{
		return false;
	}
	else if (mask == NULL)
	{
		return true;
	}
	else
	{
		INT maskIndex = (maskCoord.X % size.X) + (maskCoord.Y * size.X);	// Index of mask bit in mask byte array
		return (mask[maskIndex / 8] & (BYTE)1<<(maskIndex % 8)) > 0;
	}
}

BCE_Sprite* BCE_GameObject::getSprite()
{
	return sprite;
}

BYTE BCE_GameObject::getColliderType()
{
	return colliderType;
}

void BCE_GameObject::setPos(COORD pos)
{
	BCE_GameObject::pos = pos;
}

void BCE_GameObject::setSize(COORD size)
{
	BCE_GameObject::size = size;
}

void BCE_GameObject::setSprite(BCE_Sprite* sprite)
{
	BCE_GameObject::sprite = sprite;
}

void BCE_GameObject::setMaskBit(SHORT col, SHORT row, bool val)
{
	if (mask != NULL)
	{
		INT maskIndex = (col % size.X) + (row * size.X);	// Index of mask bit in mask byte array

		// Set bit to 0
		mask[maskIndex / 8] = mask[maskIndex / 8] & ~((BYTE)1 << (maskIndex % 8));

		// Set bit to 1 if requested
		if (val)
		{
			mask[maskIndex / 8] = mask[maskIndex / 8] | (BYTE)1 << (maskIndex % 8);
		}
		
	}
}

void BCE_GameObject::setColliderType(BYTE colliderType)
{
	BCE_GameObject::colliderType = colliderType;
}