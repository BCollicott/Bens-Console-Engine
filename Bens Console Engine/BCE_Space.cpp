#include <vector>
#include "BCE_Space.h"

BCE_Space::BCE_Space(SHORT numLayers)
{
	BCE_Space::numLayers = numLayers;
	for (int i = 0; i < numLayers; i++)
	{
		std::vector<BCE_GameObject*>* newVector = new std::vector<BCE_GameObject*>();
		layers.push_back(newVector);
	}
}

void BCE_Space::freeMemory()
{
	for (int l = 0; l < numLayers; l++)
	{
		delete(layers[l]);
	}
}

BOOL BCE_Space::addGameObject(BCE_GameObject* gameObject, SHORT layerNum)
{
	// Add gameObject to appropriate layer, if it exists
	if (0 <= layerNum && layerNum < numLayers)
	{
		layers[layerNum]->push_back(gameObject);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void BCE_Space::detectCollision(BCE_GameObject* gameObject, COORD* delta)
{
	// Only attempt to detect collision if gameObject has rectangle collisions
	if (gameObject->getColliderType() == COLLIDER_RECT)
	{

		// Search for collision in each layer
		for (int l = 0; l < numLayers; l++)
		{

			// Search for collision with each gameObject in layer
			for (int g = 0; g < layers[l]->size() ; g++)
			{
				BCE_GameObject* otherObject = (*layers[l])[g];	// Object being check for collision

				// Other object must have collider, and must be different than gameObject
				if (otherObject->getColliderType() != COLLIDER_NONE && otherObject != gameObject)
				{

					// Iterate through overlapping region in world space and detect collision if both objects have a 1 mask bit within the region
					// If there is no overlapping region, no iteration will occur
					// For GameObjects that do not have a mask, getMaskBit() always returns 1 for a coordinate that is within the the GameObject's bounds
					// this causes termination after 1 iteration if both GameObjects do not have masks, but allows for collisions between objects of different collider types

					BOOL detected = false;	// Used to break outer loops

					// Detect collisions and modify delta for movement along X axis alone
					for (int x = otherObject->spaceCoordToObjectCoord(gameObject->getPos()).X + delta->X; x < otherObject->spaceCoordToObjectCoord(gameObject->getPos()).X + gameObject->getSize().X + delta->X; x++)
					{
						for (int y = otherObject->spaceCoordToObjectCoord(gameObject->getPos()).Y; y < otherObject->spaceCoordToObjectCoord(gameObject->getPos()).Y + gameObject->getSize().Y; y++)
						{
							if (otherObject->getMaskBit(x, y) == 1)
							{
								delta->X = 0;
								detected = true;
								break;
							}
						}

						if (detected)
						{
							break;
						}
					}

					detected = false;

					// Detect collisions and modify delta for movement along Y axis given allowed movement along X axis
					for (int x = otherObject->spaceCoordToObjectCoord(gameObject->getPos()).X + delta->X; x < otherObject->spaceCoordToObjectCoord(gameObject->getPos()).X + gameObject->getSize().X + delta->X; x++)
					{
						for (int y = otherObject->spaceCoordToObjectCoord(gameObject->getPos()).Y - delta->Y; y < otherObject->spaceCoordToObjectCoord(gameObject->getPos()).Y + gameObject->getSize().Y - delta->Y; y++)
						{
							if (otherObject->getMaskBit(x, y) == 1)
							{
								delta->Y = 0;
								detected = true;
								break;
							}
						}
					}
				}
			}
		}
	}
}

std::vector<BCE_GameObject*> BCE_Space::getLayer(SHORT layerNum)
{
	return *layers[layerNum];
}

SHORT BCE_Space::getNumLayers()
{
	return numLayers;
}