#include <iostream>
#include "BCE_Panel.h"

// Constructor
BCE_Panel::BCE_Panel(BCE_Space* space, SMALL_RECT writeRegion)
{
    setSpace(space);
    setWriteRegion(writeRegion);
    setPosInSpace({ 0, 0 });

    // Instantiate buffer array according to recently calculated size and clear it
    panelBuffer = (CHAR_INFO*)malloc(panelSize.X * panelSize.Y * sizeof(CHAR_INFO));
    clearPanelBuffer();
};

void BCE_Panel::freeMemory()
{
    free(panelBuffer);
}


void BCE_Panel::clearPanelBuffer()
{
    for (int i = 0; i < panelSize.X * panelSize.Y; i++)
    {
        panelBuffer[i] = { ' ', 0 };
    }
}

void BCE_Panel::updatePanelBuffer()
{
    // Draw layers of space to buffer in order, with layers[0] being rearmost, and layers[numLayers-1] be foremost
    for (int l = 0; l < space->getNumLayers(); l++)
    {
        std::vector<BCE_GameObject*> layer = space->getLayer(l);    // Layer of space beign drawn to panel buffer

        // Draw all gameObjects in layer in order or insertion to layer (should not overlap)
        for (int g = 0; g < layer.size(); g++)
        {
            BCE_GameObject* gameObject = layer[g]; // Pointer to next GameObject to be drawn to buffer
            COORD gameObjectPos = gameObject->getPos();
            COORD gameObjectSize = gameObject->getSize();

            // Draw GameObject if it is visible within the panel according to its coordinates and the panel's position in the space
            if (gameObjectPos.X + gameObjectSize.X - 1 >= posInSpace.X && gameObjectPos.Y - (gameObjectSize.Y - 1) <= posInSpace.Y && gameObjectPos.X < posInSpace.X + panelSize.X && gameObjectPos.Y > posInSpace.Y - panelSize.Y)
            {
                // Position object is drawn to in y-down space relative to panel
                COORD panelSpace = spaceCoordToBufferCoord(gameObjectPos);

                BCE_Sprite* objectSprite = gameObject->getSprite();   // Sprite of visible GameObject to be drawn
                BYTE maskByte = (BYTE)1;

                // Draw sprite to panel buffer according to bounds of gameObject
                for (int x = 0; x < gameObjectSize.X; x++) {
                    for (int y = 0; y < gameObjectSize.Y; y++) {

                        // Draw character if its corresponding bit in the mask is 1, or there is no mask
                        if (gameObject->getMaskBit(x, y))
                        {

                            // Add character from sprite coordinates if it is visible within the panel
                            if (gameObjectPos.X + x >= posInSpace.X && gameObjectPos.Y - y <= posInSpace.Y && gameObjectPos.X + x < posInSpace.X + panelSize.X && gameObjectPos.Y - y > posInSpace.Y - panelSize.Y)
                            {
                                panelBuffer[panelSpace.X + x + (panelSpace.Y + y) * panelSize.X] = objectSprite->string[x % objectSprite->size.X + (y % objectSprite->size.Y) * objectSprite->size.X];
                            }
                        }
                    }
                }
            }
        }
    }
    
}

COORD BCE_Panel::spaceCoordToBufferCoord(COORD spaceCoord)
{
    return { spaceCoord.X - posInSpace.X, -1 * (spaceCoord.Y - posInSpace.Y) };
}

int BCE_Panel::spaceCoordToBufferIndex(COORD spaceCoord)
{
    COORD bufferCoord = spaceCoordToBufferCoord(spaceCoord);
    return bufferCoord.X + bufferCoord.Y * panelSize.X;
}

CHAR_INFO* BCE_Panel::getPanelBuffer()
{
    return panelBuffer;
}

SMALL_RECT BCE_Panel::getWriteRegion()
{
    return writeRegion;
}

COORD BCE_Panel::getPanelSize()
{
    return panelSize;
}

void BCE_Panel::setSpace(BCE_Space* space)
{
    BCE_Panel::space = space;
}

void BCE_Panel::setPosInSpace(COORD posInSpace)
{
    BCE_Panel::posInSpace = posInSpace;
}

void BCE_Panel::setWriteRegion(SMALL_RECT writeRegion)
{
    BCE_Panel::writeRegion = writeRegion;
    BCE_Panel::panelSize = { (writeRegion.Right - writeRegion.Left + 1), (writeRegion.Bottom - writeRegion.Top + 1) };
}