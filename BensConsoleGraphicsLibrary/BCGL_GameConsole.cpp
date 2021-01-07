#include <iostream>
#include <Windows.h>
#include "BCGL_GameConsole.h"
#include "BCGL_Panel.h"

// Constructor, set desired dimensions of console and mark console buffer as uninitialized
// @param width Number of character columns
// @param height Number of character rows
BCGL_GameConsole::BCGL_GameConsole(short width, short height)
{
    BCGL_GameConsole::consoleSize = { width, height };
    BCGL_GameConsole::buffer = INVALID_HANDLE_VALUE;
};

// Display console buffer in full screen, attempt to set font/console dimensions
// @param stretch If True, characters will be stretched so entire screen is used
// if False, characters will be largest possible square w/ buffer anchored to the left of the screen.
// Stretching characters is more tolerant of different buffer dimensions
// @return False if any error occured, likely due to invalid dimensions/stretch
bool BCGL_GameConsole::show(bool stretch)
{
    SMALL_RECT windowRect = { 0, 0, consoleSize.X - 1, consoleSize.Y - 1 };   // Coordnates of console window in characters, inclusive coordinates

    // Attempt to screate screen buffer with read/write access and in text mode. Cannot be inherited by child processes
    BCGL_GameConsole::buffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);

    // Check if screen buffer was successfully created
    if (buffer == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create screen buffer\n";
        return false;
    }

    // Set console buffer as the active buffer
    if (!SetConsoleActiveScreenBuffer(buffer))
    {
        std::cout << "Failed to set active screen buffer\n";
        return false;
    }

    // Set console to full screen so screen dimensions can be calculated
    if (!SetConsoleDisplayMode(buffer, CONSOLE_FULLSCREEN_MODE, nullptr))
    {
        std::cout << "Failed to set console display mode\n";
        return false;
    }

    // TODO: prompt user for desired font. FaceName all that matters, nFont/font index doesnt matter?
    // Create font info 
    // From https://stackoverflow.com/questions/35382432/how-to-change-the-console-font-size
    CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    consoleFontInfo.nFont = 0;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;
    wcscpy_s(consoleFontInfo.FaceName, L"Consolas");

    // Set font size to 1x2 pixels for measuring screen resolution
    consoleFontInfo.dwFontSize.X = 1;
    consoleFontInfo.dwFontSize.Y = 2;
    if (!SetCurrentConsoleFontEx(buffer, true, &consoleFontInfo))
    {
        std::cout << "Failed to set console font to measurement size\n";
        return false;
    }

    // Measure screen resolution by getting largest possible console dimension in characters
    // Using fullscreen and measurement font size
    COORD largestWindowSize = GetLargestConsoleWindowSize(buffer);
    if (largestWindowSize.X == 0 && largestWindowSize.Y == 0)
    {
        std::cout << "Failed to get largest window size with measurement font size\n";
        return false;
    }

    // Calculate dimensions of fullscreen window in pixels according to font size used for measurement
    short screenX = largestWindowSize.X * consoleFontInfo.dwFontSize.X;
    short screenY = largestWindowSize.Y * consoleFontInfo.dwFontSize.Y;

    // Calculate font dimensions needed to fit correct number of characters on the screen according to console size
    // Character height always proportional to screen height
    consoleFontInfo.dwFontSize.Y = screenY / consoleSize.Y;

    // Character width is proportional to screen width if stretching
    if (stretch)
    {
        consoleFontInfo.dwFontSize.X = screenX / consoleSize.X;
    }
    else
    {
        // Character width should be equal to height for largest possible square dimensions if not stretching
        consoleFontInfo.dwFontSize.X = consoleFontInfo.dwFontSize.Y;
    }

    // Set font to calculated size
    if (!SetCurrentConsoleFontEx(buffer, true, &consoleFontInfo))
    {
        std::cout << "Failed to set console font to correct size\n";
        return false;
    }

    // Set size of window in characters to dimensions according to consoleSize, after font has been finalized
    if (!SetConsoleWindowInfo(buffer, true, &windowRect))
    {
        std::cout << "Failed to set console window info\n";
        return false;
    }

    // Set size of the buffer
    if (!SetConsoleScreenBufferSize(buffer, consoleSize))
    {
        std::cout << "Failed to set console screen buffer size\n";
        return false;
    }

    // Hide cursor
    CONSOLE_CURSOR_INFO consoleCursorInfo = { 100, false };
    if (!SetConsoleCursorInfo(buffer, &consoleCursorInfo))
    {
        std::cout << "Failed to set console cursor info\n";
        return false;
    }

    return true;
};

// Add a panel to this console and draw a border around it of a specified character
// @param panel Panel to be added to console
// @return False if panel is out of bounds
bool BCGL_GameConsole::addPanel(BCGL_Panel* panel)
{
    // Determine if panel fits within console
    SMALL_RECT panelWriteRegion = panel->getWriteRegion();
    if (panelWriteRegion.Left < 0 || panelWriteRegion.Top < 0 || panelWriteRegion.Right >= consoleSize.X || panelWriteRegion.Bottom >= consoleSize.Y)
    {
        std::cout << "BCGL_Panel exceeds bounds of console\n";
        return false;
    }

    // Add panel to GameConsole's panel vector
    panels.push_back(panel);

    return true;
}

// Write characters to console buffer that form a border around a panel, not overlapping that panel's write region
// if border will exceed console bounds, it will not be drawn, and will not create an error.
// This method only needs to be called once, unless portion of the buffer containing border is drawn over or cleared somehow.
// @param borderChar Character border will consist of
// @return False if console buffer could not be written to or borderChar is NULL
bool BCGL_GameConsole::drawPanelBorder(BCGL_Panel* panel, CHAR_INFO* borderChar)
{
    // Only attempt to draw border if character was provided
    if (borderChar == nullptr)
    {
        return false;
    }
    else
    {
        // Determine dimensions of border, outside of panel
        short borderSizeX = panel->getSize().X;        // Width is flush with panel region and can safely drawn first
        short borderSizeY = panel->getSize().Y + 2;    // Height contains corners and must be shortened before drawing if top/bottom are out of bounds

        // Determine which border dimension is largest
        short largestBorderDimension;
        if (borderSizeX > borderSizeY)
        {
            largestBorderDimension = borderSizeX;
        }
        else
        {
            largestBorderDimension = borderSizeY;
        }

        CHAR_INFO* borderStr = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * largestBorderDimension);  // String of length of largest dimension that will have to be drawn

        // Populate border string with border character
        for (short i = 0; i < largestBorderDimension; i++)
        {
            borderStr[i] = *borderChar;
        }

        // Re-usable rectangles for drawing borders, initialized with dimensions of panel
        SMALL_RECT borderXWriteRegion = panel->getWriteRegion();    // To become 1-characer tall rectangle
        SMALL_RECT borderYWriteRegion = panel->getWriteRegion();    // To become 1-characer wide rectangle

        // Prepare horizontal write region for drawing top border
        borderXWriteRegion.Top--;
        borderXWriteRegion.Bottom = borderXWriteRegion.Top;

        // Prepare vertical write region for drawing left border
        borderYWriteRegion.Top--;
        borderYWriteRegion.Bottom++;
        borderYWriteRegion.Left--;
        borderYWriteRegion.Right = borderYWriteRegion.Left;

        // Do not attempt to draw top border and shorten top of vertical region if top is out of bounds
        if (borderXWriteRegion.Top < 0)
        {
            borderYWriteRegion.Top++;
        }
        else if (!WriteConsoleOutput(buffer, borderStr, { borderSizeX, 1 }, { 0, 0 }, &borderXWriteRegion))
        {
            std::cout << "Failed to draw top panel border to console output\n";
            return false;
        }

        // Adjust horizontal write region to bottom of panel
        borderXWriteRegion.Top += panel->getSize().Y + 1;
        borderXWriteRegion.Bottom = borderXWriteRegion.Top;

        // Do not attempt to draw bottom border and raise bottom of vertical region if bottom is out of bounds
        if (borderXWriteRegion.Top >= consoleSize.Y)
        {
            borderYWriteRegion.Bottom--;
        }
        else if (!WriteConsoleOutput(buffer, borderStr, { borderSizeX , 1 }, { 0, 0 }, &borderXWriteRegion))
        {
            std::cout << "Failed to draw bottom panel border to console output\n";
            return false;
        }

        // Do not attempt to draw left border if left is out of bounds
        if (!(borderYWriteRegion.Left < 0))
        {
            if (!WriteConsoleOutput(buffer, borderStr, { 1, borderSizeY }, { 0, 0 }, &borderYWriteRegion))
            {
                std::cout << "Failed to draw top panel border to console output\n";
                return false;
            }
        }

        // Adjust vertical write region to right of panel
        borderYWriteRegion.Right += panel->getSize().X + 1;
        borderYWriteRegion.Left = borderYWriteRegion.Right;

        // Do not attempt to draw right border if left is out of bounds
        if (!(borderXWriteRegion.Right >= consoleSize.X))
        {
            if (!WriteConsoleOutput(buffer, borderStr, { 1, borderSizeY }, { 0, 0 }, &borderYWriteRegion))
            {
                std::cout << "Failed to draw top panel border to console output\n";
                return false;
            }
        }

        // Free memory used for border string
        free(borderStr);

        return true;
    }
}

// Write contents to each panel's buffer to the console buffer.
// Panels must update the contents of their buffer themselves, as it may not be necessary every frame
// @return False if any panel buffer could not be written to console buffer
bool BCGL_GameConsole::updateBuffer()
{
    // Write contents of all panel buffers to console buffer
    for (short p = 0; p < panels.size(); p++)
    {
        if (!WriteConsoleOutput(buffer, panels[p]->getBuffer(), panels[p]->getSize(), { 0, 0 }, &panels[p]->getWriteRegion()))
        {
            std::cout << "Failed to write panel buffer to console output\n";
            return false;
        }
    }

    return true;
}