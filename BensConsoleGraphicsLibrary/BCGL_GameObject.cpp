#include <math.h>
#include "BCGL_GameObject.h"
#include "BCGL_Sprite.h"

BCGL_GameObject::BCGL_GameObject(COORD pos)
{
	init(pos, { 0, 0 }, nullptr);
}

BCGL_GameObject::BCGL_GameObject(COORD pos, BCGL_Sprite* sprite)
{
	init(pos, sprite->getSize(), sprite);
}

BCGL_GameObject::BCGL_GameObject(COORD pos, COORD size, BCGL_Sprite* sprite)
{
	init(pos, size, sprite);
}

void BCGL_GameObject::init(COORD pos, COORD size, BCGL_Sprite* sprite)
{
	BCGL_GameObject::pos = pos;
	BCGL_GameObject::size = size;
	BCGL_GameObject::sprite = sprite;
	BCGL_GameObject::mask = nullptr;
	BCGL_GameObject::colliderType = COLLIDER_NONE;
}

void BCGL_GameObject::transate(COORD delta)
{
	pos.X += delta.X;
	pos.Y += delta.Y;
}

COORD BCGL_GameObject::objectCoordToSpaceCoord(COORD objectCoord)
{
	return { pos.X + objectCoord.X, pos.Y - objectCoord.Y };
}

COORD BCGL_GameObject::spaceCoordToObjectCoord(COORD spaceCoord)
{
	return { spaceCoord.X - pos.X, pos.Y - spaceCoord.Y };
}

COORD BCGL_GameObject::getPos()
{
	return pos;
}

COORD BCGL_GameObject::getSize()
{
	return size;
}

BCGL_Sprite* BCGL_GameObject::getSprite()
{
	return sprite;
}

BCGL_Mask* BCGL_GameObject::getMask()
{
	return mask;
}

char BCGL_GameObject::getColliderType()
{
	return colliderType;
}

void BCGL_GameObject::setPos(COORD pos)
{
	BCGL_GameObject::pos = pos;
}

void BCGL_GameObject::setSize(COORD size)
{
	BCGL_GameObject::size = size;
}

void BCGL_GameObject::setSprite(BCGL_Sprite* sprite)
{
	BCGL_GameObject::sprite = sprite;
}

void BCGL_GameObject::setMask(BCGL_Mask* mask)
{
	BCGL_GameObject::mask = mask;
}

void BCGL_GameObject::setColliderType(char colliderType)
{
	BCGL_GameObject::colliderType = colliderType;
}