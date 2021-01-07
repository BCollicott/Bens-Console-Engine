#include <iostream>
#include "BCGL.h"

using namespace std;

BCGL_Sprite* generateColorPalette();
BCGL_Sprite* generateCharacterSet();

int main()
{
    // Define dimensions of console
    const short CONSOLE_WIDTH = 64;     // Number of colummns of characters
    const short CONSOLE_HEIGHT = 36;    // Number of rows of characters

    // Create game console
    BCGL_GameConsole gameConsole(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;
    }

    // Initialize assets under edit as square with space characters and white background
    BCGL_Sprite* spriteUnderEdit = new BCGL_Sprite({ 5, 5 });
    
    COORD iter;
    for (iter.X = 0; iter.X < spriteUnderEdit->getSize().X; iter.X++)
    {
        for (iter.Y = 0; iter.Y < spriteUnderEdit->getSize().Y; iter.Y++)
        {
            spriteUnderEdit->setCharacter(iter, { ' ', 0b01110000 });
        }
    }
    
    BCGL_GameObject objectUnderEdit({ 0, 0 }, spriteUnderEdit);

    // Create cursor and designate object it is active over
    BCGL_Sprite cursorSprite({ 1, 1 });
    cursorSprite.setCharacter({ 0, 0 }, { ' ', 0b0111 });
    BCGL_GameObject activeObject = objectUnderEdit;
    BCGL_GameObject cursorObject(objectUnderEdit.getPos(), &cursorSprite);
    

    // Create color palette assets
    BCGL_Sprite* colorPaletteSprite = generateColorPalette();
    BCGL_GameObject textColorObject({ 0, 0 }, colorPaletteSprite);
    BCGL_GameObject backgroundColorObject({ 2, 0 }, colorPaletteSprite);

    // Create character set assets
    BCGL_Sprite* characterSetSprite = generateCharacterSet();
    BCGL_GameObject characterSetObject({ 4, 0 }, characterSetSprite);

    // Add color/character assets to palette space
    BCGL_Space paletteSpace(2);
    paletteSpace.addGameObject(&textColorObject, 0);
    paletteSpace.addGameObject(&backgroundColorObject, 0);
    paletteSpace.addGameObject(&characterSetObject, 0);

    // Add assets under edit and cursor to editing space
    BCGL_Space editSpace(2);
    editSpace.addGameObject(&objectUnderEdit, 0);
    editSpace.addGameObject(&cursorObject, 1);

    // Add panels for color palettes and character set within palette space at bottom of screen
    BCGL_Char borderChar = { ' ', 0b01110000 }; // Character used for drawing borders
    BCGL_Panel textColorPanel(&paletteSpace, { 1, CONSOLE_HEIGHT - colorPaletteSprite->getSize().Y - 1, colorPaletteSprite->getSize().X, CONSOLE_HEIGHT - 2 });
    BCGL_Panel backgroundColorPanel(&paletteSpace, { textColorPanel.getWriteRegion().Right + 2, textColorPanel.getWriteRegion().Top, textColorPanel.getWriteRegion().Right + colorPaletteSprite->getSize().X + 1, textColorPanel.getWriteRegion().Bottom });
    BCGL_Panel characterSetPanel(&paletteSpace, { backgroundColorPanel.getWriteRegion().Right + 2, textColorPanel.getWriteRegion().Top, backgroundColorPanel.getWriteRegion().Right + characterSetSprite->getSize().X + 1, textColorPanel.getWriteRegion().Bottom });
    backgroundColorPanel.setPosInSpace({ backgroundColorObject.getPos() });
    characterSetPanel.setPosInSpace({ characterSetObject.getPos() });
    gameConsole.addPanel(&textColorPanel);
    gameConsole.addPanel(&backgroundColorPanel);
    gameConsole.addPanel(&characterSetPanel);
    gameConsole.drawPanelBorder(&textColorPanel, &borderChar);
    gameConsole.drawPanelBorder(&backgroundColorPanel, &borderChar);
    gameConsole.drawPanelBorder(&characterSetPanel, &borderChar);

    // Add panel displaying editing space and display it, size allowing for borders of surrounding panels
    BCGL_Panel editPanel(&editSpace, { 0, 0, CONSOLE_WIDTH - 1, textColorPanel.getWriteRegion().Top - 2 });
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
        if (!paletteSpace.detectCollision(&cursorObject, &activeObject))
        {
            cursorObject.transate({ -cursorDelta.X, -cursorDelta.Y });
        }
        
        // Update cursor sprite to color inversion of character it is hovering over
        BCGL_Char cursorChar = activeObject.getSprite()->getCharacter(activeObject.spaceCoordToObjectCoord(cursorObject.getPos()));
        cursorChar.color = (cursorChar.color ^ 0b11111111) & 0b01110111;
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
            BCGL_Sprite* tempSprite = BCGL_Sprite::deserialize("savedsprite");    // Store pointer to sprite from file so it can be safely checked if null
            if (tempSprite == nullptr)
            {
                cout << "Failed to open sprite from file\n";
            }
            else
            {
                *spriteUnderEdit = *tempSprite;
                objectUnderEdit.setSize(spriteUnderEdit->getSize());
                editPanel.clearBuffer();
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
BCGL_Sprite* generateColorPalette()
{
    BCGL_Sprite* paletteSprite = new BCGL_Sprite({ 2, 8 });

    // Iterate through sprite and set characters to spaces with colored backgrounds
    COORD iter;
    for (iter.X = 0; iter.X < paletteSprite->getSize().X; iter.X++)
    {
        for (iter.Y = 0; iter.Y < paletteSprite->getSize().Y; iter.Y++)
        {
            BCGL_Char colorChar;
            colorChar.code = ' ';

            // Background color determined by first 3 bits of Y, intensity determined by first bit of X.
            colorChar.color = ((iter.X << 3) | iter.Y) << 4;

            paletteSprite->setCharacter(iter, colorChar);
        }
    }

    return paletteSprite;
}

BCGL_Sprite* generateCharacterSet()
{
    BCGL_Sprite* charSetSprite = new BCGL_Sprite({ 32, 8 });

    COORD iter;
    for (iter.X = 0; iter.X < charSetSprite->getSize().X; iter.X++)
    {
        for (iter.Y = 0; iter.Y < charSetSprite->getSize().Y; iter.Y++)
        {
            BCGL_Char characterChar;
            characterChar.code = iter.X % charSetSprite->getSize().X + iter.Y * charSetSprite->getSize().X;

            characterChar.color = 0b0111;

            charSetSprite->setCharacter(iter, characterChar);
        }
    }

    return charSetSprite;
}
