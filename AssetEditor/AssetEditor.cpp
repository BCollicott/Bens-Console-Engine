#include <iostream>
#include "BCE.h"

using namespace std;

BCE_Sprite* generateColorPalette();

int main()
{
    // Define dimensions of console
    const short CONSOLE_WIDTH = 64;     // Number of colummns of characters
    const short CONSOLE_HEIGHT = 36;    // Number of rows of characters

    // Create game console
    BCE_GameConsole gameConsole(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;
    }

    // Initialize assets under edit
    BCE_Sprite* spriteUnderEdit = generateColorPalette();   // temporarily initialize sprite as color palette to test
    BCE_GameObject objectUnderEdit({ 0, 0 }, spriteUnderEdit);

    // Put assets under edit into editing space
    BCE_Space editSpace(2);
    editSpace.addGameObject(&objectUnderEdit, 0);

    // Add panel displaying editing space and display it
    BCE_Panel editPanel(&editSpace, { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 });
    gameConsole.addPanel(&editPanel);
    editPanel.updateBuffer();
    gameConsole.updateBuffer();

    SHORT keyMask = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT for reading key input
    while (true)
    {
        if (GetAsyncKeyState(VK_SPACE) & keyMask)
        {
            break;
        }
        Sleep(1000);
    }

    if (!spriteUnderEdit->serialize("palettesprite"))
    {
        cout << "Failed to serialize sprite under edit\n";
    }

    // Free memory and Delete assets under edit
    spriteUnderEdit->freeMemory();
    delete(spriteUnderEdit);

    editSpace.freeMemory();
    editPanel.freeMemory();
}

// Generate a 2x8 sprite containing all 16 colors, with intense colors on the bottom row
BCE_Sprite* generateColorPalette()
{
    BCE_Sprite* paletteSprite = new BCE_Sprite({ 8, 2 });

    // Iterate through sprite and set characters to spaces with colored backgrounds
    COORD iter;
    for (iter.X = 0; iter.X < 8; iter.X++)
    {
        for (iter.Y = 0; iter.Y < 2; iter.Y++)
        {
            CHAR_INFO colorChar;
            colorChar.Char.UnicodeChar = ' ';

            // Color determined by first 3 bits of X, intensity determined by first bit of Y.
            colorChar.Attributes = ((iter.Y << 3) | iter.X) << 4;

            paletteSprite->setCharacter(iter, colorChar);
        }
    }

    return paletteSprite;
}