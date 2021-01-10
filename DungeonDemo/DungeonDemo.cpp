#include <iostream>
#include <vector>
#include "BCGL.h"

BCGL_Space* createFloor(SHORT numRooms);
void destroyFloor(BCGL_Space* floor);
void quit(bool* running);

int main()
{
    // Define dimensions of console
    const SHORT CONSOLE_WIDTH = 60;     // Number of colummns of characters
    const SHORT CONSOLE_HEIGHT = 45;    // Number of rows of characters

    // Create game console
    BCGL_GameConsole gameConsole(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;
    }

    // Create spaces
    BCGL_Space textSpace(1);    // Text messages shown in lower panel
    BCGL_Space statSpace(1);    // Player/game statistics shown in right panel

    // Put first floor space as the first element in a vector of dungeon floor spaces
    std::vector<BCGL_Space*> floors; // Each floor of the dungeon is its own space
    floors.push_back(createFloor(5));

    // Test mobile object
    BCGL_Char mobileString[4] = { { 'A', 0b00000100 }, { 'B', 0b00000100 }, { 'C', 0b00000100 }, { 'D', 0b00000100 } };
    BCGL_Sprite mobileSprite(mobileString, { 2, 2 });
    BCGL_GameObject mobileObject({ 6, -12 }, {2, 2}, &mobileSprite);
    //mobileObject.setColliderType(COLLIDER_MASK);
    //mobileObject.addMask(FALSE);
    //mobileObject.setMaskBit(0, 0, TRUE);
    //mobileObject.setMaskBit(0, 5, TRUE);
    floors[0]->addGameObject(&mobileObject, 2);

    // Test static object
    BCGL_Char staticString[2] = { {'A', 0b00000010 }, {'B', 0b00000010 } };
    BCGL_Sprite staticSprite(staticString, { 1, 2 });
    BCGL_GameObject staticObject({15, -11}, { 8, 9 }, &staticSprite);
    staticObject.setColliderType(COLLIDER_RECT);
    floors[0]->addGameObject(&staticObject, 1);

    // Test object from file
    BCGL_Sprite* fileSprite = BCGL_Sprite::deserialize("palettesprite");
    if (fileSprite == nullptr)
    {
        std::cout << "Failed to load sprite from file\n";
        return 0;
    }
    BCGL_GameObject fileObject({ 20, 0 }, fileSprite);
    fileObject.setColliderType(COLLIDER_RECT);
    floors[0]->addGameObject(&fileObject, 1);

    // Create panels, text and stat panels sized to allow for borders
    SHORT textRows = 4; // Number of rows allocated to bottom text panel
    SHORT statCols = 8; // Number of cols allocated to right stats panel
    BCGL_Panel textPanel(&textSpace, { 1, CONSOLE_HEIGHT - 1 - textRows, CONSOLE_WIDTH - 2, CONSOLE_HEIGHT - 2 });
    BCGL_Panel statPanel(&statSpace, { CONSOLE_WIDTH - 1 - statCols, 1, CONSOLE_WIDTH - 2, textPanel.getWriteRegion().Top - 2 });
    BCGL_Panel gamePanel(floors[0], {1, 1, statPanel.getWriteRegion().Left - 2, textPanel.getWriteRegion().Top - 2 });
    
    // Add panels to gameConsole
    if (!gameConsole.addPanel(&textPanel) || !gameConsole.addPanel(&statPanel) || !gameConsole.addPanel(&gamePanel))
    {
        std::cout << "Failed to add panel(s) to console\n";
        return 0;
    }

    // Draw borders around all panels
    BCGL_Char borderChar = { ' ', 0b01110000 }; // Space w/ non-intense white background
    if (!gameConsole.drawPanelBorder(&textPanel, &borderChar) || !gameConsole.drawPanelBorder(&statPanel, &borderChar) || !gameConsole.drawPanelBorder(&gamePanel, &borderChar))
    {
        std::cout << "Failed to draw panel border(s) to console\n";
        return 0;
    }

    SHORT keyMask = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT for reading key input
    bool running = true;
    while (running)
    {
        COORD mobileDelta = { 0, 0 };

        mobileObject.transate(mobileDelta);

        // Apply transformation if no collisions are detected with any other GameObject in any layer
        bool collision = false;
        for (short f = 0; f < floors[0]->getNumLayers(); f++)
        {
            for (short o = 0; o < floors[0]->getLayer(f).size(); o++)
            {
                if (floors[0]->getLayer(f)[o] != &mobileObject)
                {
                    if (floors[0]->detectCollision(&mobileObject, floors[0]->getLayer(f)[o]))
                    {
                        collision = true;
                        break;
                    }
                }
            }
            if (collision)
            {
                break;
            }
        }

        if (collision)
        {
            mobileObject.transate({-1 * mobileDelta.X, -1 * mobileDelta.Y});
        }
        
        if (GetAsyncKeyState(VK_SPACE) & keyMask)
        {
            
            
            break;
        }

        // Update game panel buffer, which requires clearing due to lack of background GameObject
        gamePanel.clearBuffer();
        gamePanel.updateBuffer();

        // Update main console buffer
        gameConsole.updateBuffer();


        Sleep(1000 / 10);
    }

    // Free memory for sprite loaded from file
    fileSprite->freeMemory();
    delete(fileSprite);

    // Free memory for all floors
    for (int f = 0; f < floors.size(); f++)
    {
        destroyFloor(floors[f]);
    }

    // Free memory allocted by panels (buffers)
    textPanel.freeMemory();
    statPanel.freeMemory();
    gamePanel.freeMemory();
}

void quit(bool* running)
{
    *running = false;
}

BCGL_Space* createFloor(SHORT numRooms)
{
    BCGL_Space* newFloor = new BCGL_Space(3);   // New floor being generated w/ layers for background, statics, and mobiles    std::vector<SMALL_RECT> rooms;  // Contains dimensions of all rooms

    BCGL_Char* roomChar = (BCGL_Char*)malloc(sizeof(BCGL_Char));
    roomChar[0] = { ' ', 0b01000000 };
    BCGL_Sprite* roomSprite = new BCGL_Sprite(roomChar, { 1, 1 });
    BCGL_GameObject* roomObject = new BCGL_GameObject({ 2, -3 }, {10, 7}, roomSprite);
    roomObject->setColliderType(COLLIDER_MASK);

    // Give roomObject a mask to show only walls/perimeter
    BCGL_Mask* roomMask = new BCGL_Mask(roomObject->getSize(), false);
    COORD maskIterator; // Coordinate pair for iterating through and initializing room mask
    for (maskIterator.X = 0; maskIterator.X < roomMask->getSize().X; maskIterator.X++)
    {
        for (maskIterator.Y = 0; maskIterator.Y < roomMask->getSize().Y; maskIterator.Y++)
        {
            if (maskIterator.Y == 0 || maskIterator.Y == roomMask->getSize().Y - 1 || maskIterator.X == 0 || maskIterator.X == roomMask->getSize().X - 1)
            {
                roomMask->setBit(maskIterator, true);
            }
        }
    }
    roomObject->setMask(roomMask);

    newFloor->addGameObject(roomObject, 0);

    return newFloor;
}

void destroyFloor(BCGL_Space* floor)
{
    // Free memory for rooms
    for (int r = 0; r < floor->getLayer(0).size(); r++)
    {
        BCGL_GameObject* roomObject = floor->getLayer(0)[0];    // Room objects are unique to this space

        // Free memory for sprite's string and delete sprite itself, as it is unique to roomObject and generated in createFloor
        roomObject->getSprite()->freeMemory();
        delete(roomObject->getSprite());

        // Free memory for roomObject's mask and delete it, as it is unique to roomObject and generated in createFloor
        roomObject->getMask()->freeMemory();
        delete(roomObject->getMask());

        // Delete room object
        delete(roomObject);
    }

    // Delete floor object itself
    floor->freeMemory();
    delete(floor);
}