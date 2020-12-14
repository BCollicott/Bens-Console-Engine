#pragma once
#include <vector>
#include "BCE_GameObject.h"

// Y-Up space containing GameObjects
class BCE_Space {
	std::vector<std::vector<BCE_GameObject*>*> layers;	// Layers of GameObjects in this space. layers[0] if bottom, layers[size-1] is top
	SHORT numLayers;

	public:
		BCE_Space(SHORT numLayers);

		void freeMemory();

		void addGameObject(BCE_GameObject* gameObject, SHORT layerNum);

		std::vector<BCE_GameObject*> getLayer(SHORT layerNum);
		SHORT getNumLayers();

};