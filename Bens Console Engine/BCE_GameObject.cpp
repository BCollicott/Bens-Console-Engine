#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

BCE_GameObject::BCE_GameObject(COORD pos, BCE_Sprite sprite)
{
	BCE_GameObject::pos = pos;
	BCE_GameObject::sprite = sprite;
}