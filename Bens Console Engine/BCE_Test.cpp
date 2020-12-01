#include <iostream>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"

int main()
{
    // Create game console
    BCE_GameConsole gameConsole(40, 25);
    if (!gameConsole.show(true))
    {
        std::cout << "Failed to show game console\n";
        return 0;   
    }
    
    // Create panels in console
    BCE_Panel panel1({ 8, 11, 37, 24 }, { 30, 14 });
    BCE_Panel panel2({ 1, 1, 9, 12 }, { 9, 12 });

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
        Sleep(1000 / 30);
    }
}