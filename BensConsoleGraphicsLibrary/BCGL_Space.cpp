#include <iostream>
#include <vector>
#include "BCGL_Space.h"

// Constructor, set numLayers and allocate memory for all layer vectors
BCGL_Space::BCGL_Space(short numLayers)
{
	BCGL_Space::numLayers = numLayers;
	for (int i = 0; i < numLayers; i++)
	{
		std::vector<BCGL_GameObject*>* newLayer = new std::vector<BCGL_GameObject*>();
		layers.push_back(newLayer);
	}
}

// Free memory allocated to layer vectors
void BCGL_Space::freeMemory()
{
	for (int l = 0; l < numLayers; l++)
	{
		delete(layers[l]);
	}
}

// Add a GameObject to a specified layer vector
// @param gameObjet GameObject being added to the space
// @param layerNum Number of layer which gameObject will be added to
// @return False if layerNum is not a layer that exists in this space
bool BCGL_Space::addGameObject(BCGL_GameObject* gameObject, short layerNum)
{
	// Add gameObject to appropriate layer, if it exists
	if (0 <= layerNum && layerNum < numLayers)
	{
		layers[layerNum]->push_back(gameObject);
		return true;
	}
	else
	{
		return true;
	}
}

// Detect if a collision has occures between 2 GameObjects
// @param gameObject1 GameObject in collision
// @param gameObject2 GameObject in collision
// @return True if bounds (and a mask bit) of GameObjects overlap
bool BCGL_Space::detectCollision(BCGL_GameObject* gameObject1, BCGL_GameObject* gameObject2)
{
	SMALL_RECT overlap;	// Region in space in which the two GameObjects overlap

	// Initialize overlap as bounds of gameObject1
	overlap.Left = gameObject1->getPos().X;
	overlap.Top = gameObject1->getPos().Y;
	overlap.Right = gameObject1->getPos().X + gameObject1->getSize().X - 1;
	overlap.Bottom = gameObject1->getPos().Y - gameObject1->getSize().Y + 1;

	// Update overlap bounds to those of mobileObject if they are the actual constraints of the overlap
	if (gameObject2->getPos().X > overlap.Left)
	{
		overlap.Left = gameObject2->getPos().X;
	}
	if (gameObject2->getPos().Y < overlap.Top)
	{
		overlap.Top = gameObject2->getPos().Y;
	}
	if (gameObject2->getPos().X + gameObject2->getSize().X - 1 < overlap.Right)
	{
		overlap.Right = gameObject2->getPos().X + gameObject2->getSize().X - 1;
	}
	if (gameObject2->getPos().Y - gameObject2->getSize().Y + 1 > overlap.Bottom)
	{
		overlap.Bottom = gameObject2->getPos().Y - gameObject2->getSize().Y + 1;
	}

	// Return false if GameObjects are disjoint, without risking iteration through X dimension in loop below
	if (overlap.Right < overlap.Left || overlap.Top < overlap.Bottom)
	{
		return false;
	}

	// If either GameObject has a mask, iteration through the overlapping region must occur
	// if neither GameObject has a mask, true will be returned in first iteration
	COORD overlapIterator;	// Coord in space for iterating through masks in overlapping region
	for (overlapIterator.X = overlap.Left; overlapIterator.X <= overlap.Right; overlapIterator.X++)
	{
		for (overlapIterator.Y = overlap.Top; overlapIterator.Y >= overlap.Bottom; overlapIterator.Y--)
		{
			if ((gameObject1->getMask() == nullptr || gameObject1->getMask()->getBit(gameObject1->spaceCoordToObjectCoord(overlapIterator)) == true)
				&& (gameObject2->getMask() == nullptr || gameObject2->getMask()->getBit(gameObject2->spaceCoordToObjectCoord(overlapIterator)) == true))
			{
				return true;
			}
		}
	}

	return false;
}

std::vector<BCGL_GameObject*> BCGL_Space::getLayer(short layerNum)
{
	return *layers[layerNum];
}

short BCGL_Space::getNumLayers()
{
	return numLayers;
}