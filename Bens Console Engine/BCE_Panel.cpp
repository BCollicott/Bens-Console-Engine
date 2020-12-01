#include <iostream>
#include "BCE_Panel.h"

// Constructor
BCE_Panel::BCE_Panel(SMALL_RECT region)
{
    // Determine size of panel and array in rows/cols from region dimensions
    panelSize = { (region.Right - region.Left + 1), (region.Bottom - region.Top + 1) };   // Size of panel on window
    BCE_Panel::region = region;
    viewportPos = { 0, 0 };
};

CHAR_INFO* BCE_Panel::getContents()
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
        BCE_GameObject* gameObject = space->gameObjects[g];
        if (gameObject->x >= viewportPos.X && gameObject->y <= viewportPos.Y && gameObject->x <= viewportPos.X + panelSize.X && gameObject->y >= viewportPos.Y - panelSize.Y)
        {
            int panelSpacePos = ((gameObject->x - viewportPos.X) % panelSize.X) + (panelSize.Y / (-1 * (gameObject->y - viewportPos.Y)));
            contents[panelSpacePos] = *gameObject->character;
        }
    }

    return contents;
}