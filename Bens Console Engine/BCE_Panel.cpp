#include <iostream>
#include "BCE_Panel.h"

// Constructor
BCE_Panel::BCE_Panel(BCE_Space* space, SMALL_RECT writeRegion)
{
    setSpace(space);
    setWriteRegion(writeRegion);
    setPosInSpace({ 0, 0 });
};

CHAR_INFO* BCE_Panel::getSpaceArray()
{
    // Create empty array to represent panel contents
    CHAR_INFO* contents = new CHAR_INFO[panelSize.X * panelSize.Y];
    for (int i = 0; i < panelSize.X * panelSize.Y; i++)
    {
        contents[i] = { ' ', 0 };
    }

    // Draw all GameObjects of space to proper panel space
    for (int g = 0; g < space->gameObjects.size(); g++)
    {
        // Get pointer to next GameObject to be draw to array
        BCE_GameObject* gameObject = space->gameObjects[g];

        // Draw GameObject if it is visible within the panel according to its coordinates and the panel's position in the space
        if (gameObject->pos.X >= posInSpace.X && gameObject->pos.Y <= posInSpace.Y && gameObject->pos.X < posInSpace.X + panelSize.X && gameObject->pos.Y > posInSpace.Y - panelSize.Y)
        {
            int panelSpacePos = ((gameObject->pos.X - posInSpace.X) % panelSize.X) + (panelSize.X * (-1 * (gameObject->pos.Y - posInSpace.Y)));
            for (int r = 0; r < gameObject->sprite->size.Y; r++) {
                for (int c = 0; c < gameObject->sprite->size.X; c++) {
                    contents[(panelSpacePos + c) + (panelSpacePos + r * panelSize.X)] = gameObject->sprite->string[c + r * panelSize.X];
                }
            }
        }
    }

    return contents;
}

SMALL_RECT BCE_Panel::getWriteRegion()
{
    return writeRegion;
}

COORD BCE_Panel::getPanelSize()
{
    return panelSize;
}

void BCE_Panel::setSpace(BCE_Space* space)
{
    BCE_Panel::space = space;
}

void BCE_Panel::setPosInSpace(COORD posInSpace)
{
    BCE_Panel::posInSpace = posInSpace;
}

void BCE_Panel::setWriteRegion(SMALL_RECT writeRegion)
{
    BCE_Panel::writeRegion = writeRegion;
    BCE_Panel::panelSize = { (writeRegion.Right - writeRegion.Left + 1), (writeRegion.Bottom - writeRegion.Top + 1) };
}