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

void BCE_Space::addGameObject(BCE_GameObject* gameObject, SHORT layerNum)
{
	layers[layerNum]->push_back(gameObject);
}

std::vector<BCE_GameObject*> BCE_Space::getLayer(SHORT layerNum)
{
	return *layers[layerNum];
}

SHORT BCE_Space::getNumLayers()
{
	return numLayers;
}