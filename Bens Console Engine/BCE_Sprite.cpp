#include <Windows.h>
#include "BCE_Sprite.h"

BCE_Sprite::BCE_Sprite(CHAR_INFO* string, COORD size)
{
	BCE_Sprite::string = string;
	BCE_Sprite::size = size;
}