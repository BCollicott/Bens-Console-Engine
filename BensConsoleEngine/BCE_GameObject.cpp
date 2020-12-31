#include <math.h>
#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

BCE_GameObject::BCE_GameObject(COORD pos)
{
	init(pos, { 0, 0 }, nullptr);
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
	BCE_GameObject::mask = nullptr;
	BCE_GameObject::colliderType = COLLIDER_NONE;
}

void BCE_GameObject::transate(COORD delta)
{
	pos.X += delta.X;
	pos.Y += delta.Y;
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

BCE_Sprite* BCE_GameObject::getSprite()
{
	return sprite;
}

BCE_Mask* BCE_GameObject::getMask()
{
	return mask;
}

char BCE_GameObject::getColliderType()
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

void BCE_GameObject::setMask(BCE_Mask* mask)
{
	BCE_GameObject::mask = mask;
}

void BCE_GameObject::setColliderType(char colliderType)
{
	BCE_GameObject::colliderType = colliderType;
}