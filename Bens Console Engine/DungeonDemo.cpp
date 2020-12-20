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
    const SHORT CONSOLE_WIDTH = 60;     // Number of colummns of characters
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
    BCE_GameObject mobileObject({ 6, -6 }, {2, 2}, &mobileSprite);
    mobileObject.setColliderType(COLLIDER_RECT);
    floors[0]->addGameObject(&mobileObject, 2);

    // Test static object
    CHAR_INFO staticString[2] = { {'A', 0b00000010 }, {'B', 0b00000010 } };
    BCE_Sprite staticSprite(staticString, { 1, 2 });
    BCE_GameObject staticObject({15, -5}, { 8, 9 }, &staticSprite);
    staticObject.setColliderType(COLLIDER_RECT);
    floors[0]->addGameObject(&staticObject, 1);

    // Test static object 2
    CHAR_INFO staticString2[2] = { {'C', 0b00000010 }, {'D', 0b00000010 } };
    BCE_Sprite staticSprite2(staticString2, { 1, 2 });
    BCE_GameObject staticObject2({ 20, 0 }, { 8, 8 }, &staticSprite2);
    staticObject2.setColliderType(COLLIDER_RECT);
    floors[0]->addGameObject(&staticObject2, 1);

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

    SHORT keyMask = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT for reading key input
    while (true)
    {
        COORD mobileDelta = { 0, 0 };
        if (GetAsyncKeyState(VK_LEFT) & keyMask)
        {
            mobileDelta.X--;
        }
        if (GetAsyncKeyState(VK_RIGHT) & keyMask)
        {
            mobileDelta.X++;
        }
        if (GetAsyncKeyState(VK_UP) & keyMask)
        {
            mobileDelta.Y++;
        }
        if (GetAsyncKeyState(VK_DOWN) & keyMask)
        {
            mobileDelta.Y--;
        }

        // Detect collisions and modify mobileDelta before applying transformation
        floors[0]->detectCollision(&mobileObject, &mobileDelta);
        mobileObject.transate(mobileDelta);

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

        gamePanel.clearPanelBuffer();
        gameConsole.updateConsoleBuffer();
        Sleep(1000 / 10);
    }
}

BCE_Space* createFloor(SHORT numRooms)
{
    BCE_Space* newFloor = new BCE_Space(3);   // New floor being generated w/ layers for background, statics, and mobiles    std::vector<SMALL_RECT> rooms;  // Contains dimensions of all rooms

    CHAR_INFO* roomChar = (CHAR_INFO*)malloc(sizeof(CHAR_INFO));
    roomChar[0] = { ' ', 0b01000000 };
    BCE_Sprite* roomSprite = new BCE_Sprite(roomChar, { 1, 1 });
    BCE_GameObject* roomObject = new BCE_GameObject({ 2, -3 }, {10, 7}, roomSprite);
    roomObject->setColliderType(COLLIDER_MASK);

    // Give roomObject a mask to show only walls/perimeter
    roomObject->addMask(false);
    COORD roomSize = roomObject->getSize();
    for (SHORT x = 0; x < roomSize.X; x++)
    {
        for (SHORT y = 0; y < roomSize.Y; y++)
        {
            if (y == 0 || y == roomSize.Y - 1 || x == 0 || x == roomSize.X - 1)
            {
                roomObject->setMaskBit(x, y, true);
            }
        }
    }

    newFloor->addGameObject(roomObject, 0);

    return newFloor;
}

void destroyFloor(BCE_Space* floor)
{
    // Free memory for rooms
    for (int r = 0; r < floor->getLayer(0).size(); r++)
    {
        BCE_GameObject* roomObject = floor->getLayer(0)[0];    // Room objects are unique to this space

        // Free memory for sprite's string and delete sprite itself, as it is unique to roomObject
        roomObject->getSprite()->freeMemory();
        delete(roomObject->getSprite());

        // Free memory for roomObject's mask
        roomObject->freeMask();  
    }

    // Delete floor object itself
    floor->freeMemory();
    delete(floor);
}