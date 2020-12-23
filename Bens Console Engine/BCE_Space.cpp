#include <iostream>
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
	if (gameObject->getColliderType() != COLLIDER_NONE)
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

					SMALL_RECT overlap; // Overlapping region after movement along both axes, inclusive coordinates

					// Initialize overlap as bounds of gameObject
					overlap.Left = gameObject->getPos().X + delta->X;
					overlap.Top = gameObject->getPos().Y + delta->Y;
					overlap.Right = gameObject->getPos().X + gameObject->getSize().X + delta->X - 1;
					overlap.Bottom = gameObject->getPos().Y - gameObject->getSize().Y + delta->Y + 1;

					// Update bounds to those of otherObject if they are the actual constraints of the overlap
					if (otherObject->getPos().X > overlap.Left)
					{
						overlap.Left = otherObject->getPos().X;
					}
					if (otherObject->getPos().Y < overlap.Top)
					{
						overlap.Top = otherObject->getPos().Y;
					}
					if (otherObject->getPos().X + otherObject->getSize().X - 1 < overlap.Right)
					{
						overlap.Right = otherObject->getPos().X + otherObject->getSize().X - 1;
					}
					if (otherObject->getPos().Y - otherObject->getSize().Y + 1 > overlap.Bottom)
					{
						overlap.Bottom = otherObject->getPos().Y - otherObject->getSize().Y + 1;
					}

					BOOL detected = FALSE;	// Used to break outer loops

					// Detect collisions and modify delta for movement along X axis alone
					for (SHORT x = overlap.Left; x <= overlap.Right; x++)
					{
						for (SHORT y = overlap.Top - delta->Y; y >= overlap.Bottom - delta->Y; y--)
						{

							if (gameObject->getMaskBit(gameObject->spaceCoordToObjectCoord({ x - delta->X, y })) == TRUE && otherObject->getMaskBit(otherObject->spaceCoordToObjectCoord({ x, y })) == TRUE)
							{

								// Update overlap region if its horizontal bounds were determines by now-nullified delta X value
								if (overlap.Left == gameObject->getPos().X + delta->X)
								{
									overlap.Left -= delta->X;
								}

								if (overlap.Right == gameObject->getPos().X + gameObject->getSize().X + delta->X - 1)
								{
									overlap.Right -= delta->X;
								}

								// Nullify delta X value
								delta->X = 0;

								// Break inner and outer loop
								detected = TRUE;
								break;
							}
						}

						if (detected)
						{
							break;
						}
					}

					detected = FALSE;

					// Detect collisions and modify delta for movement along Y axis given allowed movement along X axis
					for (SHORT x = overlap.Left; x <= overlap.Right; x++)
					{
						for (SHORT y = overlap.Top; y >= overlap.Bottom; y--)
						{
							if (gameObject->getMaskBit(gameObject->spaceCoordToObjectCoord({ x - delta->X, y - delta->Y })) == TRUE && otherObject->getMaskBit(otherObject->spaceCoordToObjectCoord({ x, y })) == TRUE)
							{
								delta->Y = 0;
								detected = TRUE;
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