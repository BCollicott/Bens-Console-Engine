#include <iostream>
#include <vector>
#include "BCE_Space.h"

// Constructor, set numLayers and allocate memory for all layer vectors
BCE_Space::BCE_Space(short numLayers)
{
	BCE_Space::numLayers = numLayers;
	for (int i = 0; i < numLayers; i++)
	{
		std::vector<BCE_GameObject*>* newLayer = new std::vector<BCE_GameObject*>();
		layers.push_back(newLayer);
	}
}

// Free memory allocated to layer vectors
void BCE_Space::freeMemory()
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
bool BCE_Space::addGameObject(BCE_GameObject* gameObject, short layerNum)
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

// Detect if a collision has occures between 2 GameObjects without care for intruding region or mobile/static relationship
// @param gameObject1 GameObject in collision
// @param gameObject2 GameObject in collision
// @return True if bounds (and a mask bit) of GameObjects overlap
bool BCE_Space::detectCollision(BCE_GameObject* gameObject1, BCE_GameObject* gameObject2)
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
			if (gameObject2->getMaskBit(gameObject2->spaceCoordToObjectCoord(overlapIterator)) == true && gameObject1->getMaskBit(gameObject1->spaceCoordToObjectCoord(overlapIterator)) == true)
			{
				return true;
			}
		}
	}

	return false;
}

std::vector<BCE_GameObject*> BCE_Space::getLayer(short layerNum)
{
	return *layers[layerNum];
}

short BCE_Space::getNumLayers()
{
	return numLayers;
}