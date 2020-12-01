#include <iostream>
#include <Windows.h>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"

BCE_GameConsole::BCE_GameConsole(short width, short height)
{
    consoleSize = { width, height };    // Dimensions of console window in number of rows/cols
};

bool BCE_GameConsole::show(bool stretchToFit)
{
    SMALL_RECT windowRect = { 0, 0, consoleSize.X - 1, consoleSize.Y - 1 };   // Coordnates of console window in characters, inclusive

    // Create screen buffer
    HANDLE screenBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL);
    if (screenBuffer == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create screen buffer\n";
        return false;
    }

    // Set screen buffer to be console's active buffer
    if (!SetConsoleActiveScreenBuffer(screenBuffer))
    {
        std::cout << "Failed to set active screen buffer\n";
        return false;
    }

    // Set console to full screen
    if (!SetConsoleDisplayMode(screenBuffer, CONSOLE_FULLSCREEN_MODE, NULL))
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
    if (!SetCurrentConsoleFontEx(screenBuffer, true, &consoleFontInfo))
    {
        std::cout << "Failed to set console font to measurement size\n";
        return false;
    }

    // Measure window/screen dimensions using 1x2 font size
    COORD largestWindowSize = GetLargestConsoleWindowSize(screenBuffer);
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
    if (!SetCurrentConsoleFontEx(screenBuffer, true, &consoleFontInfo))
    {
        std::cout << "Failed to set console font to correct size\n";
        return false;
    }

    // Set size of window in characters to the dimenions allowed by buffer
    if (!SetConsoleWindowInfo(screenBuffer, true, &windowRect))
    {
        std::cout << "Failed to set console window info\n";
        return false;
    }

    // Set size of the buffer
    if (!SetConsoleScreenBufferSize(screenBuffer, consoleSize))
    {
        std::cout << "Failed to set console screen buffer size\n";
        return false;
    }

    // Hide cursor
    CONSOLE_CURSOR_INFO consoleCursorInfo = { 100, false };
    if (!SetConsoleCursorInfo(screenBuffer, &consoleCursorInfo))
    {
        std::cout << "Failed to set console cursor info\n";
        return false;
    }

    // Set newly created and displayed screenbuffer as GameConsole's member
    BCE_GameConsole::screenBuffer = screenBuffer;

    return true;
};

bool BCE_GameConsole::addPanel(BCE_Panel* panel)
{
    // Determine if panel fits within console
    if (panel->region.Left < 0 || panel->region.Top < 0 || panel->region.Right >= consoleSize.X || panel->region.Bottom >= consoleSize.Y)
    {
        std::cout << "BCE_Panel exceeds bounds of console\n";
        return false;
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
bool BCE_GameConsole::update()
{
    // Write all panels within gameConsole to output
    for (int p = 0; p < panels.size(); p++)
    {
        BCE_Panel* panel = panels[p];   // BCE_Panel from gameConsole
        if (!WriteConsoleOutput(screenBuffer, panel->getContents(), panel->panelSize, { 0, 0 }, &panel->region))
        {
            std::cout << "Failed to write panel contents to console output\n";
        }
    }

    return true;
}