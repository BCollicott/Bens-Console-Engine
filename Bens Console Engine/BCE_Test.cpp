#include <iostream>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"
#include "BCE_Space.h"
#include "BCE_GameObject.h"

int main()
{
    // Create game console
    BCE_GameConsole gameConsole(40, 25);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;   
    }
    
    BCE_Panel panel({ 1, 1, 38, 23 });
    //BCE_Panel panel2({ 5, 0, 9, 4 });
    BCE_Space space;
    CHAR_INFO testCharacter = { 'B', FOREGROUND_GREEN };
    CHAR_INFO originCharacter = { 'O', FOREGROUND_BLUE };
    BCE_GameObject testObject(1, -1, &testCharacter);
    BCE_GameObject origin(0, 0, &originCharacter);

    space.addGameObject(&testObject);
    space.addGameObject(&origin);
    panel.space = &space;
    //panel2.space = &space;

    //panel2.viewportPos = { 1, 1 };

    // Add panels to console
    if (!gameConsole.addPanel(&panel))
    {
        std::cout << "Failed to add panel 1 to console\n";
        return 0;
    }
    /*
    if (!gameConsole.addPanel(&panel2))
    {
        std::cout << "Failed to add panel 2 to console\n";
        return 0;
    }*/

    // Display console
    gameConsole.update();

    // Close when spacebar is pressed
    while (true) {
        if (GetKeyState(VK_SPACE) & (int)pow(2, (sizeof(SHORT) * 8) - 1)) 
        {
            break;
        }
        if (GetKeyState(VK_LEFT) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.x--;
        }
        if (GetKeyState(VK_RIGHT) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.x++;
        }
        if (GetKeyState(VK_UP) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.y++;
        }
        if (GetKeyState(VK_DOWN) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.y--;
        }
        gameConsole.update();
        Sleep(1000 / 30);
    }
}