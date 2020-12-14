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

            // Draw GameObject if it is visible within the panel according to its coordinates and the panel's position in the space
            if (gameObject->pos.X + gameObject->size.X - 1 >= posInSpace.X && gameObject->pos.Y - (gameObject->size.Y - 1) <= posInSpace.Y && gameObject->pos.X < posInSpace.X + panelSize.X && gameObject->pos.Y > posInSpace.Y - panelSize.Y)
            {
                // Position object is drawn to in y-down space relative to panel
                COORD panelSpace = spaceToBufferCoord(gameObject->pos);

                BCE_Sprite* objectSprite = gameObject->getSprite();   // Sprite of visible GameObject to be drawn

                // Draw sprite to panel buffer according to bounds of gameObject
                for (int r = 0; r < gameObject->size.Y; r++) {
                    for (int c = 0; c < gameObject->size.X; c++) {

                        // Add character from sprite coordinates if it is visible within the panel
                        if (gameObject->pos.X + c >= posInSpace.X && gameObject->pos.Y - r <= posInSpace.Y && gameObject->pos.X + c< posInSpace.X + panelSize.X && gameObject->pos.Y - r > posInSpace.Y - panelSize.Y)
                        {
                            panelBuffer[panelSpace.X + c + (panelSpace.Y + r) * panelSize.X] = objectSprite->string[c % objectSprite->size.X + (r % objectSprite->size.Y) * objectSprite->size.X];
                        }
                    }
                }
            }
        }
    }
    
}

COORD BCE_Panel::spaceToBufferCoord(COORD spaceCoord)
{
    return { spaceCoord.X - posInSpace.X, -1 * (spaceCoord.Y - posInSpace.Y) };
}

int BCE_Panel::spaceToBufferIndex(COORD spaceCoord)
{
    COORD bufferCoord = spaceToBufferCoord(spaceCoord);
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