#include <iostream>
#include <vector>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"
#include "BCE_Space.h"
#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

BCE_Space* createFloor(SHORT numRooms);
void destroyFloor(BCE_Space* floor);

int main()
{
    // Define dimensions of console
    const SHORT CONSOLE_WIDTH = 80;     // Number of colummns of characters
    const SHORT CONSOLE_HEIGHT = 45;    // Number of rows of characters

    // Create game console
    BCE_GameConsole gameConsole(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;
    }

    // Create spaces
    BCE_Space textSpace(1);    // Text messages shown in lower panel
    BCE_Space statSpace(1);    // Player/game statistics shown in right panel

    // Put first floor space as the first element in a vector of dungeon floor spaces
    std::vector<BCE_Space*> floors; // Each floor of the dungeon is its own space
    floors.push_back(createFloor(5));

    // Test mobile object
    CHAR_INFO mobileString = { 'M', 0b00000100 };
    BCE_Sprite mobileSprite(&mobileString, { 1,1 });
    BCE_GameObject mobileObject({ 6, -6 }, &mobileSprite);
    floors[0]->addGameObject(&mobileObject, 2);

    // Test static object
    CHAR_INFO staticString[2] = { {'A', 0b00000010 }, {'B', 0b00000010 } };
    BCE_Sprite staticSprite(staticString, { 1, 2 });
    BCE_GameObject staticObject({5, -5}, { 8, 9 }, &staticSprite);
    floors[0]->addGameObject(&staticObject, 1);

    // Create panels, text and stat panels sized to allow for borders
    SHORT textRows = 4; // Number of rows allocated to bottom text panel
    SHORT statCols = 8; // Number of cols allocated to right stats panel
    BCE_Panel textPanel(&textSpace, { 1, CONSOLE_HEIGHT - 1 - textRows, CONSOLE_WIDTH - 2, CONSOLE_HEIGHT - 2 });
    BCE_Panel statPanel(&statSpace, { CONSOLE_WIDTH - 1 - statCols, 1, CONSOLE_WIDTH - 2, textPanel.getWriteRegion().Top - 2 });
    BCE_Panel gamePanel(floors[0], {1, 1, statPanel.getWriteRegion().Left - 2, textPanel.getWriteRegion().Top - 2 });
    
    // Add panels to gameConsole with solid white borders
    CHAR_INFO borderChar = { ' ', 0b01110000 }; // Space w/ non-intense white background
    if (!gameConsole.addPanel(&textPanel, &borderChar) || !gameConsole.addPanel(&statPanel, &borderChar) || !gameConsole.addPanel(&gamePanel, &borderChar))
    {
        std::cout << "Failed to add panel(s) to console\n";
        return 0;
    }

    gameConsole.updateConsoleBuffer();

    // Quit game when space bar is pressed
    SHORT keyMask = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT for reading key input
    while (true)
    {
        if (GetAsyncKeyState(VK_SPACE) & keyMask)
        {
            // Free memory for all floors
            for (int f = 0; f < floors.size(); f++)
            {
                destroyFloor(floors[f]);
            }

            // Free memory allocted by panels (buffers)
            textPanel.freeMemory();
            statPanel.freeMemory();
            gamePanel.freeMemory();
            
            break;
        }
    }
}

BCE_Space* createFloor(SHORT numRooms)
{
    BCE_Space* newFloor = new BCE_Space(3);   // New floor being generated w/ layers for background, statics, and mobiles

    CHAR_INFO* floorChar = (CHAR_INFO*)malloc(sizeof(CHAR_INFO));
    floorChar[0] = { 'F', 0b00000111 };                             // Simple 1x1 repeating sprite to indicate floor background
    BCE_Sprite* floorSprite = new BCE_Sprite(floorChar, {1, 1});
    BCE_GameObject* floorObject = new BCE_GameObject({ 0, 0 }, { 50, 50 }, floorSprite);

    newFloor->addGameObject(floorObject, 0);

    return newFloor;
}

void destroyFloor(BCE_Space* floor)
{
    BCE_GameObject* floorObject = floor->getLayer(0)[0];    // Only GameObject in background layer is the floor itself, which is unique to this space

    // Free memory for sprite's string and delete sprite itself, as it is unique to floorObject
    floorObject->getSprite()->freeMemory();
    delete(floorObject->getSprite());

    // Delete floor object itself
    floor->freeMemory();
    delete(floor);
}