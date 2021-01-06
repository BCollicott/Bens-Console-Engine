#include <iostream>
#include "BCE.h"

using namespace std;

// Unicode values of Code page 437/Original character set of IBM PC at indices of their alt code
// Printible ASCII characters' alt codes are the same as their original ascii value, so their char values can be used to retrieve them
const unsigned short CHARACTER_SET[256] = 
{
    0x0000, 0x263a, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, 0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
    0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8, 0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC,
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302,
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0,
};

BCE_Sprite* generateColorPalette();
BCE_Sprite* generateCharacterSet();

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

    // Initialize assets under edit as square with space characters and white background
    BCE_Sprite* spriteUnderEdit = new BCE_Sprite({ 5, 5 });
    
    COORD iter;
    for (iter.X = 0; iter.X < spriteUnderEdit->getSize().X; iter.X++)
    {
        for (iter.Y = 0; iter.Y < spriteUnderEdit->getSize().Y; iter.Y++)
        {
            spriteUnderEdit->setCharacter(iter, { ' ', 0b01110000 });
        }
    }
    
    BCE_GameObject objectUnderEdit({ 0, 0 }, spriteUnderEdit);

    // Create cursor
    BCE_Sprite cursorSprite({ 1, 1 });
    cursorSprite.setCharacter({ 0, 0 }, { ' ', 0b0111 });
    BCE_GameObject cursorObject({ 1, -1 }, &cursorSprite);

    // Create color palette assets
    BCE_Sprite* colorPaletteSprite = generateColorPalette();
    BCE_GameObject textColorObject({ 0, 0 }, colorPaletteSprite);
    BCE_GameObject backgroundColorObject({ 2, 0 }, colorPaletteSprite);

    // Create character set assets
    BCE_Sprite* characterSetSprite = generateCharacterSet();
    BCE_GameObject characterSetObject({ 4, 0 }, characterSetSprite);

    // Add color/character assets to palette space
    BCE_Space paletteSpace(2);
    paletteSpace.addGameObject(&textColorObject, 0);
    paletteSpace.addGameObject(&backgroundColorObject, 0);
    paletteSpace.addGameObject(&characterSetObject, 0);

    // Add assets under edit and cursor to editing space
    BCE_Space editSpace(2);
    editSpace.addGameObject(&objectUnderEdit, 0);
    editSpace.addGameObject(&cursorObject, 1);

    // Add panels for color palettes and character set within palette space at bottom of screen
    CHAR_INFO borderChar = { ' ', 0b01110000 }; // Character used for drawing borders
    BCE_Panel textColorPanel(&paletteSpace, { 1, CONSOLE_HEIGHT - colorPaletteSprite->getSize().Y - 1, colorPaletteSprite->getSize().X, CONSOLE_HEIGHT - 2 });
    BCE_Panel backgroundColorPanel(&paletteSpace, { textColorPanel.getWriteRegion().Right + 2, textColorPanel.getWriteRegion().Top, textColorPanel.getWriteRegion().Right + colorPaletteSprite->getSize().X + 1, textColorPanel.getWriteRegion().Bottom });
    BCE_Panel characterSetPanel(&paletteSpace, { backgroundColorPanel.getWriteRegion().Right + 2, textColorPanel.getWriteRegion().Top, backgroundColorPanel.getWriteRegion().Right + characterSetSprite->getSize().X + 1, textColorPanel.getWriteRegion().Bottom });
    backgroundColorPanel.setPosInSpace({ backgroundColorObject.getPos() });
    characterSetPanel.setPosInSpace({ characterSetObject.getPos() });
    gameConsole.addPanel(&textColorPanel);
    gameConsole.addPanel(&backgroundColorPanel);
    gameConsole.addPanel(&characterSetPanel);
    gameConsole.drawPanelBorder(&textColorPanel, &borderChar);
    gameConsole.drawPanelBorder(&backgroundColorPanel, &borderChar);
    gameConsole.drawPanelBorder(&characterSetPanel, &borderChar);

    // Add panel displaying editing space and display it, size allowing for borders of surrounding panels
    BCE_Panel editPanel(&editSpace, { 0, 0, CONSOLE_WIDTH - 1, textColorPanel.getWriteRegion().Top - 2 });
    gameConsole.addPanel(&editPanel);
    
    // Begin main loop
    SHORT keyMask = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT for reading key input
    while (true)
    {
        // Get cursor movement from input
        COORD cursorDelta = { 0, 0 };
        if (GetAsyncKeyState(VK_LEFT) & keyMask)
        {
            cursorDelta.X -= 1;
        }
        if (GetAsyncKeyState(VK_RIGHT) & keyMask)
        {
            cursorDelta.X += 1;
        }
        if (GetAsyncKeyState(VK_UP) & keyMask)
        {
            cursorDelta.Y += 1;
        }
        if (GetAsyncKeyState(VK_DOWN) & keyMask)
        {
            cursorDelta.Y -= 1;
        }
        cursorObject.transate(cursorDelta);

        
        // Update cursor sprite to color inversion of character it is hovering over
        CHAR_INFO cursorChar = spriteUnderEdit->getCharacter(objectUnderEdit.spaceCoordToObjectCoord(cursorObject.getPos()));
        cursorChar.Attributes = (char)~cursorChar.Attributes;
        cursorSprite.setCharacter({ 0, 0 }, cursorChar);
        

        // Draw to sprite if space pressed
        if (GetAsyncKeyState(VK_SPACE) & keyMask)
        {
            if (editSpace.detectCollision(&cursorObject, &objectUnderEdit))
            {
                spriteUnderEdit->setCharacter(objectUnderEdit.spaceCoordToObjectCoord(cursorObject.getPos()), { ' ', 0b01100000 });
            }
        }

        // Save sprite if ctrl+S pressed
        if ((GetAsyncKeyState(VK_CONTROL) & keyMask) && (GetAsyncKeyState(0x53) & keyMask))
        {
            if (!spriteUnderEdit->serialize("savedsprite"))
            {
                cout << "Failed to serialize sprite under edit\n";
            }
        }

        // Load sprite if ctrl+O pressed
        if ((GetAsyncKeyState(VK_CONTROL) & keyMask) && (GetAsyncKeyState(0x4F) & keyMask))
        {
            BCE_Sprite* tempSprite = BCE_Sprite::deserialize("savedsprite");    // Store pointer to sprite from file so it can be safely checked if null
            if (tempSprite == nullptr)
            {
                cout << "Failed to open sprite from file\n";
            }
            else
            {
                *spriteUnderEdit = *tempSprite;
                objectUnderEdit.setSize(spriteUnderEdit->getSize());
            }
        }
        
        // Exit main loop if crtl+Q pressed
        if ((GetAsyncKeyState(VK_CONTROL) & keyMask) && (GetAsyncKeyState(0x51) & keyMask))
        {
            break;
        }

        textColorPanel.updateBuffer();
        backgroundColorPanel.updateBuffer();
        characterSetPanel.updateBuffer();
        editPanel.updateBuffer();
        gameConsole.updateBuffer();

        Sleep(100);
    }
    

    // Free memory and Delete assets under edit and palette assets
    spriteUnderEdit->freeMemory();
    delete(spriteUnderEdit);
    colorPaletteSprite->freeMemory();
    delete(colorPaletteSprite);
    characterSetSprite->freeMemory();
    delete(characterSetSprite);

    editSpace.freeMemory();
    editPanel.freeMemory();
}

// Generate a 2x8 sprite containing all 16 colors, with intense colors on the bottom row
BCE_Sprite* generateColorPalette()
{
    BCE_Sprite* paletteSprite = new BCE_Sprite({ 2, 8 });

    // Iterate through sprite and set characters to spaces with colored backgrounds
    COORD iter;
    for (iter.X = 0; iter.X < paletteSprite->getSize().X; iter.X++)
    {
        for (iter.Y = 0; iter.Y < paletteSprite->getSize().Y; iter.Y++)
        {
            CHAR_INFO colorChar;
            colorChar.Char.UnicodeChar = ' ';

            // Background color determined by first 3 bits of Y, intensity determined by first bit of X.
            colorChar.Attributes = ((iter.X << 3) | iter.Y) << 4;

            paletteSprite->setCharacter(iter, colorChar);
        }
    }

    return paletteSprite;
}

BCE_Sprite* generateCharacterSet()
{
    BCE_Sprite* charSetSprite = new BCE_Sprite({ 32, 8 });

    COORD iter;
    for (iter.X = 0; iter.X < charSetSprite->getSize().X; iter.X++)
    {
        for (iter.Y = 0; iter.Y < charSetSprite->getSize().Y; iter.Y++)
        {
            CHAR_INFO characterChar;
            characterChar.Char.UnicodeChar = CHARACTER_SET[iter.X % charSetSprite->getSize().X + iter.Y * charSetSprite->getSize().X];

            characterChar.Attributes = 0b0111;

            charSetSprite->setCharacter(iter, characterChar);
        }
    }

    return charSetSprite;
}
