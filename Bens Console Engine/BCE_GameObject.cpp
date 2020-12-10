#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

BCE_GameObject::BCE_GameObject(COORD pos, COORD size, BCE_Sprite* sprite)
{
	BCE_GameObject::pos = pos;
	BCE_GameObject::size = size;
	BCE_GameObject::sprite = sprite;
}

BCE_GameObject::BCE_GameObject(COORD pos, BCE_Sprite* sprite)
{
	BCE_GameObject::pos = pos;
	BCE_GameObject::size = sprite->size;
	BCE_GameObject::sprite = sprite;
}