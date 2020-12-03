#include <iostream>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"
#include "BCE_Space.h"
#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

int main()
{
    // Create game console
    BCE_GameConsole gameConsole(40, 20);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;   
    }
    //BCE_Panel panel2({ 5, 0, 9, 4 });
    
    
    CHAR_INFO testCharacter[4] = { 
        { '-', FOREGROUND_BLUE | FOREGROUND_RED }, { '|', FOREGROUND_BLUE | FOREGROUND_RED } , 
        { '\\', FOREGROUND_BLUE | FOREGROUND_RED }, { '/', FOREGROUND_BLUE | FOREGROUND_RED } };
        

    /*
    CHAR_INFO testCharacter[1] = {
        { 'A', FOREGROUND_BLUE | FOREGROUND_RED } };
    */

    CHAR_INFO originCharacter[1] = { { 'O', FOREGROUND_BLUE } };
    
    BCE_Sprite testSprite(testCharacter, { 2, 2 });
    BCE_Sprite originSprite(originCharacter, { 1, 1 });
    
    BCE_GameObject testObject({ 2, -1 }, testSprite);
    BCE_GameObject origin({ 0, 0 }, originSprite);

    BCE_Space space;
    space.addGameObject(&testObject);
    space.addGameObject(&origin);

    BCE_Panel panel1(&space, { 20, 0, 39, 9 });
    BCE_Panel panel2(&space, { 0, 0, 19, 9 });
    BCE_Panel panel3(&space, { 0, 10, 19, 19 });
    BCE_Panel panel4(&space, { 20, 10, 39, 19 });
    
    panel1.setPosInSpace({ 0, 9 });
    panel2.setPosInSpace({ -19, 9 });
    panel3.setPosInSpace({ -19, 0 });
    panel4.setPosInSpace({ 0, 0 });

    // Add panels to console
    if (!gameConsole.addPanel(&panel1) || !gameConsole.addPanel(&panel2) || !gameConsole.addPanel(&panel3) || !gameConsole.addPanel(&panel4))
    {
        std::cout << "Failed to add paneto console\n";
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

        // Clear buffer of both panels due to lack of background GameObject
        panel1.clearPanelBuffer();
        panel2.clearPanelBuffer();
        panel3.clearPanelBuffer();
        panel4.clearPanelBuffer();

        gameConsole.update();
        Sleep(1000 / 10);
    }
}