#include <iostream>
#include "BCE_Panel.h"

// Constructor
BCE_Panel::BCE_Panel(SMALL_RECT region, COORD arraySize)
{
    // Determine size of panel and array in rows/cols from region dimensions
    panelSize = { (region.Right - region.Left + 1), (region.Bottom - region.Top + 1) };   // Size of panel on window
    BCE_Panel::arraySize = arraySize;

    // If either dimension of array is less than the corresponding dimension of the panel, center that dimension in the panel
    if (arraySize.X < panelSize.X)
    {
        region.Left = panelSize.X / 2 - arraySize.X / 2;
        region.Right = region.Left + arraySize.X;
    }
    if (arraySize.Y < panelSize.Y)
    {
        region.Top = panelSize.Y / 2 - arraySize.Y / 2;
        region.Bottom = region.Top + arraySize.Y;
    }

    // Create array of characters and fill with space characters
    CHAR_INFO* panelArray = new CHAR_INFO[arraySize.X * arraySize.Y];
    for (int i = 0; i < arraySize.X * arraySize.Y; i++)
    {
        if (i == arraySize.X * arraySize.Y - 1) {
            panelArray[i] = { 'B', FOREGROUND_GREEN };
            break;
        }
        panelArray[i] = { 'A', FOREGROUND_BLUE };
    }

    BCE_Panel::region = region;
    viewportPos = { 0, 0 };
    array = panelArray;
};