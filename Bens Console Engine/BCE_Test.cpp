#include <iostream>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"
#include "BCE_Space.h"
#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

int main()
{
    // Create game console
    BCE_GameConsole gameConsole(40, 25);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;   
    }
    //BCE_Panel panel2({ 5, 0, 9, 4 });
    
    CHAR_INFO* testCharacter = new CHAR_INFO[4]{ { 'A', FOREGROUND_BLUE | FOREGROUND_RED }, { 'A', FOREGROUND_BLUE | FOREGROUND_RED } , {'B', FOREGROUND_BLUE | FOREGROUND_RED }, {'B', FOREGROUND_BLUE | FOREGROUND_RED } };
    //CHAR_INFO* originCharacter = { 'O', FOREGROUND_BLUE };
    
    BCE_Sprite testSprite(testCharacter, { 2, 2 });
    BCE_GameObject testObject({ 1, -1 }, &testSprite);
    //BCE_GameObject origin({ 0, 0 }, originCharacter);

    BCE_Space space;
    space.addGameObject(&testObject);
    //space.addGameObject(&origin);

    BCE_Panel panel1(&space, { 0, 0, 19, 24 });
    BCE_Panel panel2(&space, { 20, 0, 39, 24 });
    panel2.setPosInSpace({ -10, 15 });

    // Add panels to console
    if (!gameConsole.addPanel(&panel1))
    {
        std::cout << "Failed to add panel 1 to console\n";
        return 0;
    }
    
    if (!gameConsole.addPanel(&panel2))
    {
        std::cout << "Failed to add panel 2 to console\n";
        return 0;
    }

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
            testObject.pos.X--;
        }
        if (GetKeyState(VK_RIGHT) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.pos.X++;
        }
        if (GetKeyState(VK_UP) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.pos.Y++;
        }
        if (GetKeyState(VK_DOWN) & (int)pow(2, (sizeof(SHORT) * 8) - 1))
        {
            testObject.pos.Y--;
        }
        gameConsole.update();
        Sleep(1000 / 10);
    }
}