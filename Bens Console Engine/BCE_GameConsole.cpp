#include <iostream>
#include <Windows.h>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"

BCE_GameConsole::BCE_GameConsole(short width, short height)
{
    consoleSize = { width, height };    // Dimensions of console window in number of rows/cols
    consoleBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
};

bool BCE_GameConsole::show(bool stretchToFit)
{
    SMALL_RECT windowRect = { 0, 0, consoleSize.X - 1, consoleSize.Y - 1 };   // Coordnates of console window in characters, inclusive

    // Check if screen buffer was successfully created in constructor
    if (consoleBuffer == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create screen buffer\n";
        return false;
    }

    // Set screen buffer to be console's active buffer
    if (!SetConsoleActiveScreenBuffer(consoleBuffer))
    {
        std::cout << "Failed to set active screen buffer\n";
        return false;
    }

    // Set console to full screen
    if (!SetConsoleDisplayMode(consoleBuffer, CONSOLE_FULLSCREEN_MODE, NULL))
    {
        std::cout << "Failed to set console display mode\n";
        return false;
    }

    // Create font info 
    // From https://stackoverflow.com/questions/35382432/how-to-change-the-console-font-size
    CONSOLE_FONT_INFOEX consoleFontInfo;
    consoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    consoleFontInfo.nFont = 0;
    consoleFontInfo.FontFamily = FF_DONTCARE;
    consoleFontInfo.FontWeight = FW_NORMAL;
    wcscpy_s(consoleFontInfo.FaceName, L"Consolas");

    // Set font size to be able to measure resolution of console/screen. 
    consoleFontInfo.dwFontSize.X = 1;
    consoleFontInfo.dwFontSize.Y = 2;
    if (!SetCurrentConsoleFontEx(consoleBuffer, true, &consoleFontInfo))
    {
        std::cout << "Failed to set console font to measurement size\n";
        return false;
    }

    // Measure window/screen dimensions using 1x2 font size
    COORD largestWindowSize = GetLargestConsoleWindowSize(consoleBuffer);
    if (largestWindowSize.X == 0 && largestWindowSize.Y == 0)
    {
        std::cout << "Failed to get largest window size with measurement font size\n";
        return false;
    }

    // Calculate dimensions of fullscreen window in pixels
    int screenX = largestWindowSize.X * consoleFontInfo.dwFontSize.X;
    int screenY = largestWindowSize.Y * consoleFontInfo.dwFontSize.Y;

    consoleFontInfo.dwFontSize.Y = screenY / consoleSize.Y;
    if (stretchToFit)
    {
        consoleFontInfo.dwFontSize.X = screenX / consoleSize.X;
    }
    else
    {
        consoleFontInfo.dwFontSize.X = consoleFontInfo.dwFontSize.Y;
    }

    // Set font to correct size to fit screen
    if (!SetCurrentConsoleFontEx(consoleBuffer, true, &consoleFontInfo))
    {
        std::cout << "Failed to set console font to correct size\n";
        return false;
    }

    // Set size of window in characters to the dimenions allowed by buffer
    if (!SetConsoleWindowInfo(consoleBuffer, true, &windowRect))
    {
        std::cout << "Failed to set console window info\n";
        return false;
    }

    // Set size of the buffer
    if (!SetConsoleScreenBufferSize(consoleBuffer, consoleSize))
    {
        std::cout << "Failed to set console screen buffer size\n";
        return false;
    }

    // Hide cursor
    CONSOLE_CURSOR_INFO consoleCursorInfo = { 100, false };
    if (!SetConsoleCursorInfo(consoleBuffer, &consoleCursorInfo))
    {
        std::cout << "Failed to set console cursor info\n";
        return false;
    }

    // Set newly created and displayed screenbuffer as GameConsole's member
    BCE_GameConsole::consoleBuffer = consoleBuffer;

    return true;
};

bool BCE_GameConsole::addPanel(BCE_Panel* panel, CHAR_INFO* borderChar)
{
    // Determine if panel fits within console
    SMALL_RECT panelWriteRegion = panel->getWriteRegion();
    if (panelWriteRegion.Left < 0 || panelWriteRegion.Top < 0 || panelWriteRegion.Right >= consoleSize.X || panelWriteRegion.Bottom >= consoleSize.Y)
    {
        std::cout << "BCE_Panel exceeds bounds of console\n";
        return false;
    }

    // Draw border if requested
    if (borderChar != NULL)
    {
        // Create arrays to represent horizontal and vertical border around panel
        SHORT borderSizeX = panel->getPanelSize().X;        // Width is flush with panel region and can safely drawn first
        SHORT borderSizeY = panel->getPanelSize().Y + 2;    // Height contains corners and must be shortened before drawing if top/bottom are out of bounds
        CHAR_INFO* borderStr;                               // String containing borderChar for drawing borders

        // Determine which border dimension is largest
        SHORT largestBorderDimension;
        if (borderSizeX > borderSizeY)
        {
            largestBorderDimension = borderSizeX;
        }
        else
        {
            largestBorderDimension = borderSizeY;
        }

        // Set border string to length of larger dimension
        borderStr = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * largestBorderDimension);

        // Populate border string with border character
        for (int i = 0; i < largestBorderDimension; i++)
        {
            borderStr[i] = *borderChar;
        }

        // Re-usable rectangles for drawing borders
        SMALL_RECT borderXWriteRegion = panel->getWriteRegion();   
        SMALL_RECT borderYWriteRegion = panel->getWriteRegion();

        // Prepare horizontal write region for drawing top border
        borderXWriteRegion.Top--;
        borderXWriteRegion.Bottom = borderXWriteRegion.Top;

        // Prepare vertical write region for drawing left border
        borderYWriteRegion.Top--;
        borderYWriteRegion.Bottom++;
        borderYWriteRegion.Left--;
        borderYWriteRegion.Right = borderYWriteRegion.Left;

        // Attempt to draw top border
        if (borderXWriteRegion.Top < 0)
        {
            borderYWriteRegion.Top++;
        }
        else if (!WriteConsoleOutput(consoleBuffer, borderStr, { borderSizeX, 1 }, { 0, 0 }, &borderXWriteRegion))
        {
            std::cout << "Failed to draw top panel border to console output\n";
        }

        // Attempt to draw bottom border
        borderXWriteRegion.Top += panel->getPanelSize().Y + 1;
        borderXWriteRegion.Bottom = borderXWriteRegion.Top;
        if (borderXWriteRegion.Top >= consoleSize.Y)
        {
            borderYWriteRegion.Bottom--;
        }
        else if (!WriteConsoleOutput(consoleBuffer, borderStr, { borderSizeX + 2 , 1 }, { 0, 0 }, &borderXWriteRegion))
        {
            std::cout << "Failed to draw bottom panel border to console output\n";
        }

        // Attempt to draw left border
        if (!(borderYWriteRegion.Left < 0))
        {
            if (!WriteConsoleOutput(consoleBuffer, borderStr, { 1, borderSizeY }, { 0, 0 }, &borderYWriteRegion))
            {
                std::cout << "Failed to draw top panel border to console output\n";
            }
        }

        // Attempt to draw bottom border
        borderYWriteRegion.Right += panel->getPanelSize().X + 1;
        borderYWriteRegion.Left = borderYWriteRegion.Right;
        if (!(borderXWriteRegion.Right >= consoleSize.X))
        {
            if (!WriteConsoleOutput(consoleBuffer, borderStr, { 1, borderSizeY }, { 0, 0 }, &borderYWriteRegion))
            {
                std::cout << "Failed to draw top panel border to console output\n";
            }
        }

        free(borderStr);
    }

    // Add panel to GameConsole's panel vector
    panels.push_back(panel);

    return true;
}

/*
 * Draw all panels in BCE_GameConsole struct to the screen buffer/console window
 * @param gameConsole BCE_GameConsole struct whose panels are drawn
 * @return False if windows console write method fails
 */
bool BCE_GameConsole::updateConsoleBuffer()
{
    // Write all panels within gameConsole to output
    for (int p = 0; p < panels.size(); p++)
    {
        BCE_Panel* panel = panels[p];   // BCE_Panel from gameConsole
        panel->updatePanelBuffer();
        if (!WriteConsoleOutput(consoleBuffer, panel->getPanelBuffer(), panel->getPanelSize(), { 0, 0 }, &panel->getWriteRegion()))
        {
            std::cout << "Failed to write panel buffer to console output\n";
        }
    }

    return true;
}