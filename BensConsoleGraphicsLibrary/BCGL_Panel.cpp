#include <iostream>
#include "BCGL_Panel.h"

// Constructor, initialize instance variables and set up buffer
BCGL_Panel::BCGL_Panel(BCGL_Space* space, SMALL_RECT writeRegion)
{
    // Initialize or calculate instance variables
    setSpace(space);
    setWriteRegion(writeRegion);
    setPosInSpace({ 0, 0 });

    // Allocate memory for buffer array according to recently calculated size and clear it
    buffer = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    clearBuffer();
};

// Convert BCGL_Char to Windows CHAR_INFO struct and add it to panel buffer
void BCGL_Panel::setBufferChar(COORD panelCoord, BCGL_Char bufferChar)
{
    buffer[panelCoord.X + panelCoord.Y * size.X] = { CHARACTER_SET[bufferChar.code], bufferChar.color };
}

// Free memory allocated by class
void BCGL_Panel::freeMemory()
{
    // Panel buffer allocated in constructor
    free(buffer);
}

// Fill panel buffer with spaces
void BCGL_Panel::clearBuffer()
{
    for (int i = 0; i < size.X * size.Y; i++)
    {
        buffer[i] = { ' ', 0 };
    }
}

// Write sprites of all GameObjects in region of space visible by panel to the panel buffer
void BCGL_Panel::updateBuffer()
{
    // Draw layers of space to buffer in order, with layers[0] being rearmost, and layers[numLayers-1] be foremost
    for (short l = 0; l < space->getNumLayers(); l++)
    {
        std::vector<BCGL_GameObject*> layer = space->getLayer(l);    // Layer of space being drawn to panel buffer

        // Draw all gameObjects in layer in order of insertion to layer (should not overlap)
        for (short g = 0; g < layer.size(); g++)
        {
            BCGL_GameObject* gameObject = layer[g]; // Pointer to next GameObject to be drawn to buffer
            COORD gameObjectPos = gameObject->getPos();
            COORD gameObjectSize = gameObject->getSize();

            // Draw GameObject if it is visible within the panel according to its coordinates and the panel's position in the space
            if (gameObjectPos.X + gameObjectSize.X - 1 >= posInSpace.X
                && gameObjectPos.Y - (gameObjectSize.Y - 1) <= posInSpace.Y
                && gameObjectPos.X < posInSpace.X + size.X
                && gameObjectPos.Y > posInSpace.Y - size.Y)
            {
                // Draw sprite to panel buffer according to bounds of gameObject
                COORD spriteIterator;   // Coordinate pair for iterating over region gameObject's sprite is written to
                for (spriteIterator.X = gameObjectPos.X; spriteIterator.X < gameObjectPos.X + gameObjectSize.X; spriteIterator.X++) {
                    for (spriteIterator.Y = gameObjectPos.Y; spriteIterator.Y > gameObjectPos.Y - gameObjectSize.Y; spriteIterator.Y--) {

                        // Write character from sprite to buffer if bit in gameObject's mask at iterator's position in space is 1, or there is no mask
                        if (gameObject->getMask() == nullptr || gameObject->getMask()->getBit(gameObject->spaceCoordToObjectCoord(spriteIterator)))
                        {
                            // Write character to buffer if sprite iterator is not outside region of space shown by panel
                            if (spriteIterator.X >= posInSpace.X
                                && spriteIterator.Y <= posInSpace.Y
                                && spriteIterator.X < posInSpace.X + size.X
                                && spriteIterator.Y > posInSpace.Y - size.Y)
                            {
                                setBufferChar(spaceCoordToBufferCoord(spriteIterator), gameObject->getSprite()->getCharacter(gameObject->spaceCoordToObjectCoord(spriteIterator)));
                            }
                        }
                    }
                }
            }
        }
    }
}

// Covert a coordinate from its position in Y-up space to its position in this panel's Y-down buffer
COORD BCGL_Panel::spaceCoordToBufferCoord(COORD spaceCoord)
{
    return { spaceCoord.X - posInSpace.X, -1 * (spaceCoord.Y - posInSpace.Y) };
}

CHAR_INFO* BCGL_Panel::getBuffer()
{
    return buffer;
}

CHAR_INFO BCGL_Panel::getBufferChar(COORD panelCoord)
{
    return buffer[panelCoord.X + panelCoord.Y * size.X];
}

SMALL_RECT BCGL_Panel::getWriteRegion()
{
    return writeRegion;
}

COORD BCGL_Panel::getSize()
{
    return size;
}

void BCGL_Panel::setSpace(BCGL_Space* space)
{
    BCGL_Panel::space = space;
}

void BCGL_Panel::setPosInSpace(COORD posInSpace)
{
    BCGL_Panel::posInSpace = posInSpace;
}

// Set writeRegion and recalculate size from its dimensions
void BCGL_Panel::setWriteRegion(SMALL_RECT writeRegion)
{
    BCGL_Panel::writeRegion = writeRegion;
    BCGL_Panel::size = { (writeRegion.Right - writeRegion.Left + 1), (writeRegion.Bottom - writeRegion.Top + 1) };
}