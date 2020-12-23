#include <iostream>
#include "BCE_Panel.h"

// Constructor, initialize instance variables and set up buffer
BCE_Panel::BCE_Panel(BCE_Space* space, SMALL_RECT writeRegion)
{
    // Initialize or calculate instance variables
    setSpace(space);
    setWriteRegion(writeRegion);
    setPosInSpace({ 0, 0 });

    // Allocate memory for buffer array according to recently calculated size and clear it
    buffer = (CHAR_INFO*)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    clearBuffer();
};

// Free memory allocated by class
void BCE_Panel::freeMemory()
{
    // Panel buffer allocated in constructor
    free(buffer);
}

// Fill panel buffer with spaces
void BCE_Panel::clearBuffer()
{
    for (int i = 0; i < size.X * size.Y; i++)
    {
        buffer[i] = { ' ', 0 };
    }
}

// Write sprites of all GameObjects in region of space visible by panel to the panel buffer
void BCE_Panel::updateBuffer()
{
    // Draw layers of space to buffer in order, with layers[0] being rearmost, and layers[numLayers-1] be foremost
    for (short l = 0; l < space->getNumLayers(); l++)
    {
        std::vector<BCE_GameObject*> layer = space->getLayer(l);    // Layer of space being drawn to panel buffer

        // Draw all gameObjects in layer in order of insertion to layer (should not overlap)
        for (short g = 0; g < layer.size(); g++)
        {
            BCE_GameObject* gameObject = layer[g]; // Pointer to next GameObject to be drawn to buffer
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
                        if (gameObject->getMaskBit(gameObject->spaceCoordToObjectCoord(spriteIterator)))
                        {
                            // Write character to buffer if sprite iterator is not outside region of space shown by panel
                            if (spriteIterator.X >= posInSpace.X
                                && spriteIterator.Y <= posInSpace.Y
                                && spriteIterator.X < posInSpace.X + size.X
                                && spriteIterator.Y > posInSpace.Y - size.Y)
                            {
                                buffer[spaceCoordToBufferIndex(spriteIterator)] = gameObject->getSprite()->getCharacter(gameObject->spaceCoordToObjectCoord(spriteIterator));
                            }
                        }
                    }
                }
            }
        }
    }
}

// Covert a coordinate from its position in Y-up space to its position in this panel's Y-down buffer
COORD BCE_Panel::spaceCoordToBufferCoord(COORD spaceCoord)
{
    return { spaceCoord.X - posInSpace.X, -1 * (spaceCoord.Y - posInSpace.Y) };
}

// Covert a coordinate from its position in Y-up space to its index in this panel's buffer, if it is to br written there
int BCE_Panel::spaceCoordToBufferIndex(COORD spaceCoord)
{
    COORD bufferCoord = spaceCoordToBufferCoord(spaceCoord);
    return bufferCoord.X + bufferCoord.Y * size.X;
}

CHAR_INFO* BCE_Panel::getBuffer()
{
    return buffer;
}

SMALL_RECT BCE_Panel::getWriteRegion()
{
    return writeRegion;
}

COORD BCE_Panel::getSize()
{
    return size;
}

void BCE_Panel::setSpace(BCE_Space* space)
{
    BCE_Panel::space = space;
}

void BCE_Panel::setPosInSpace(COORD posInSpace)
{
    BCE_Panel::posInSpace = posInSpace;
}

// Set writeRegion and recalculate size from its dimensions
void BCE_Panel::setWriteRegion(SMALL_RECT writeRegion)
{
    BCE_Panel::writeRegion = writeRegion;
    BCE_Panel::size = { (writeRegion.Right - writeRegion.Left + 1), (writeRegion.Bottom - writeRegion.Top + 1) };
}