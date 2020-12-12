#include <iostream>
#include <queue>
#include "BCE_GameConsole.h"
#include "BCE_Panel.h"
#include "BCE_Space.h"
#include "BCE_GameObject.h"
#include "BCE_Sprite.h"

int snakeDemo(SHORT consoleWidth, SHORT consoleHeight, bool stretch);
int miscTest();

int main()
{
    return snakeDemo(25, 20, false);
    //return miscTest();
}

int miscTest()
{
    // Create game console
    BCE_GameConsole gameConsole(40, 25);
    if (!gameConsole.show(false))
    {
        std::cout << "Failed to show game console\n";
        return 0;
    }
    //BCE_Panel panel2({ 5, 0, 9, 4 });


    CHAR_INFO testCharacter[4] = {
        { 'A', 0b10100110 }, { 'B', 0b10100110 } ,
        { 'C', 0b10100110 }, { 'D', 0b10100110 } };


    /*
    CHAR_INFO testCharacter[1] = {
        { 'A', FOREGROUND_BLUE | FOREGROUND_RED } };
    */

    CHAR_INFO originCharacter[1] = { { 'O', FOREGROUND_BLUE } };

    BCE_Sprite testSprite(testCharacter, { 2, 2 });
    BCE_Sprite originSprite(originCharacter, { 1, 1 });

    BCE_GameObject testObject({ 2, -1 }, { 8, 4 }, &testSprite);
    BCE_GameObject origin({ 0, 0 }, &originSprite);

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

    if (!gameConsole.addPanel(&panel1, NULL) || !gameConsole.addPanel(&panel2, NULL) || !gameConsole.addPanel(&panel3, NULL) || !gameConsole.addPanel(&panel4, NULL))
    {
        std::cout << "Failed to add paneto console\n";
        return 0;
    }

    // Display console
    gameConsole.updateConsoleBuffer();

    // Get input using mask to determine if key was pressed after last call, or is currently pressed
    SHORT maskMSBLSB = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT
    while (true) {
        if (GetAsyncKeyState(VK_SPACE) & maskMSBLSB)
        {
            break;
        }
        if (GetAsyncKeyState(VK_LEFT) & maskMSBLSB)
        {
            testObject.pos.X--;
        }
        if (GetAsyncKeyState(VK_RIGHT) & maskMSBLSB)
        {
            testObject.pos.X++;
        }
        if (GetAsyncKeyState(VK_UP) & maskMSBLSB)
        {
            testObject.pos.Y++;
        }
        if (GetAsyncKeyState(VK_DOWN) & maskMSBLSB)
        {
            testObject.pos.Y--;
        }

        // Clear buffer of both panels due to lack of background GameObject
        panel1.clearPanelBuffer();
        panel2.clearPanelBuffer();
        panel3.clearPanelBuffer();
        panel4.clearPanelBuffer();

        gameConsole.updateConsoleBuffer();
        Sleep(1000 / 10);
    }
}

int snakeDemo(SHORT consoleWidth, SHORT consoleHeight, bool stretch)
{
    // Create game console
    BCE_GameConsole gameConsole(consoleWidth, consoleHeight);
    if (!gameConsole.show(stretch))
    {
        std::cout << "Failed to show game console\n";
        return 0;
    }

    // Head of snake controlled by player
    CHAR_INFO snakeString = { ' ', BACKGROUND_GREEN };
    BCE_Sprite snakeSprite(&snakeString, { 1, 1 });
    BCE_GameObject snakeObject({ consoleWidth / 2, consoleHeight / 2 }, &snakeSprite);

    // Food located at random position on level
    CHAR_INFO foodString = { 0x2665, FOREGROUND_RED };
    BCE_Sprite foodSprite(&foodString, { 1, 1 });
    BCE_GameObject foodObject({ 0, 0 }, &foodSprite);

    // Blank used to erase tail of snake
    CHAR_INFO clearString = { ' ', 0 };
    BCE_Sprite clearSprite(&clearString, { 1, 1 });
    BCE_GameObject clearObject({ 0, 0 }, &clearSprite);

    // Char used to draw border of gameplay panel
    CHAR_INFO borderChar = { ' ', 0b01110000 };

    // Label for score
    char scoreLabelString[] = "Score:";
    BCE_Sprite scoreLabelSprite(scoreLabelString, 0b00000111, { 6, 1 });
    BCE_GameObject scoreLabelObject({ 0, 0 }, &scoreLabelSprite);

    // Value of score
    BCE_Sprite scoreValueSprite({ consoleWidth / 2 - 1 + consoleWidth % 2 - scoreLabelSprite.size.X, 1 });
    //scoreValueSprite.setValue(0, 0b00000100, '0');
    BCE_GameObject scoreValueObject({ scoreLabelObject.pos.X + scoreLabelObject.size.X, 0 }, &scoreValueSprite);

    // Label for time
    char timeLabelString[] = "Time:";
    BCE_Sprite timeLabelSprite(timeLabelString, 0b00000111, { 5, 1 });
    BCE_GameObject timeLabelObject({ consoleWidth / 2 + consoleWidth % 2, 0 }, &timeLabelSprite);

    // Value of time
    BCE_Sprite timeValueSprite({ consoleWidth / 2 - timeLabelSprite.size.X, 1 });
    BCE_GameObject timeValueObject({ timeLabelObject.pos.X + timeLabelObject.size.X, 0 }, &timeValueSprite);

    // Space with gameplay objects
    BCE_Space gameSpace;
    gameSpace.addGameObject(&snakeObject);
    gameSpace.addGameObject(&clearObject);
    gameSpace.addGameObject(&foodObject);

    // Space with text info
    BCE_Space textSpace;
    textSpace.addGameObject(&scoreLabelObject);
    textSpace.addGameObject(&scoreValueObject);
    textSpace.addGameObject(&timeLabelObject);
    textSpace.addGameObject(&timeValueObject);

    // Fullscreen panel displaying positive X and Y coordinate space
    BCE_Panel gamePanel(&gameSpace, { 1, 1, consoleWidth - 2, consoleHeight - 3 });
    gamePanel.setPosInSpace({ 0, gamePanel.getPanelSize().Y - 1 });
    if (!gameConsole.addPanel(&gamePanel, &borderChar))
    {
        std::cout << "Failed to add game panel to console\n";
        return 0;
    }
    
    BCE_Panel textPanel(&textSpace, { 0, consoleHeight - 1, consoleWidth - 1, consoleHeight - 1 });
    if (!gameConsole.addPanel(&textPanel, NULL))
    {
        std::cout << "Failed to add text panel to console\n";
        return 0;
    }

    // Queue of all previous snake head positions that make up tail
    std::queue<COORD> tailQueue;
    tailQueue.push(snakeObject.pos);

    // Set up variables for gameplay
    SHORT keyMask = (SHORT)pow(2, (sizeof(SHORT) * 8) - 1) | (SHORT)1;   // Mask containing most sigificant bit and least significant bit of SHORT for reading key input
    COORD delta = { 1, 0 };                     // Transformation applied to player each frame
    foodObject.pos.X = snakeObject.pos.X + 3;   // Starting x position of food
    foodObject.pos.Y = snakeObject.pos.Y;       // Starting y position of food
    int score = 0;                              // Number of food pieces eaten and used to increase fps
    int timeSec = 0;                            // Time elapsed in full seconds
    int timeMs = 0;                             // Time elapsed since last full second

    // Display intital score and time values
    scoreValueSprite.setValue(score, 0b00000010, '0');
    timeValueSprite.setValue(timeSec, 0b00000100, '0');

    // Main loop
    while (true) {

        COORD lastDelta = delta;    // Transformation applied last frame

        // Read input to get requested direction
        if (GetAsyncKeyState(VK_LEFT) & keyMask)
        {
            delta = { -1, 0 };
        }
        if (GetAsyncKeyState(VK_RIGHT) & keyMask)
        {
            delta = { 1, 0 };
        }
        if (GetAsyncKeyState(VK_UP) & keyMask)
        {
            delta = { 0, 1 };
        }
        if (GetAsyncKeyState(VK_DOWN) & keyMask)
        {
            delta = { 0, -1 };
        }

        // Do not change direction if new delta is along same axis as last delta (X or Y coords are both 0)
        if (lastDelta.X == delta.X || lastDelta.Y == delta.Y)
        {
            delta = lastDelta;
        }

        // Apply transformation
        snakeObject.pos.X += delta.X;
        snakeObject.pos.Y += delta.Y;

        // Add new head position to back of tail queue
        tailQueue.push(snakeObject.pos);

        // End game if player exceeded a boundary or ate its tail (backgorund color/arrtibutes match snake string)
        if (snakeObject.pos.X < 0 || snakeObject.pos.Y < 0 || snakeObject.pos.X >= gamePanel.getPanelSize().X || snakeObject.pos.Y >= gamePanel.getPanelSize().Y
            || gamePanel.getPanelBuffer()[gamePanel.spaceToBufferIndex(snakeObject.pos)].Attributes == snakeString.Attributes)
        {
            break;
        }
        else if (snakeObject.pos.X == foodObject.pos.X && snakeObject.pos.Y == foodObject.pos.Y)
        {
            // Calculate new position of food if it was eaten, and do not move tail, which will increase length
            do
            {
                foodObject.pos.X = rand() % gamePanel.getPanelSize().X;
                foodObject.pos.Y = rand() % gamePanel.getPanelSize().Y;
            } while (gamePanel.getPanelBuffer()[gamePanel.spaceToBufferIndex(foodObject.pos)].Attributes == snakeString.Attributes);


            // Increment score and update display
            score++;
            scoreValueSprite.setValue(score, 0b00000010, '0');
        }
        else
        {
            // Clear end of tail at position indicated by front of queue
            clearObject.pos = tailQueue.front();
            tailQueue.pop();
        }

        // Update timeMs to time about to be slept/frame length and adjust it and timeSec if a full second has elapsed
        timeMs += 1000 / (5 + score / 5);
        if (timeMs >= 1000)
        {
            timeSec += timeMs / 1000;
            timeMs %= 1000;

            // Update timer display if timeSec has a new full value
            timeValueSprite.setValue(timeSec, 0b00000100, '0');
        }

        gameConsole.updateConsoleBuffer();
        Sleep(1000 / (5 + score / 5));
    }

    // Quit game when space bar is pressed
    while (true)
    {
        if (GetAsyncKeyState(VK_SPACE) & keyMask)
        {
            gamePanel.destroy();
            textPanel.destroy();
            break;
        }
    }
}