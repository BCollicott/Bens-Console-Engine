#pragma once
#include <vector>
#include "BCE_GameObject.h"

#define NO_COLLISION {0, 0, -1, -1}	// Returned by detectCollision when its parameters are not overlapping

// Y-Up space containing GameObjects in 1 or more layers
class BCE_Space {
	std::vector<std::vector<BCE_GameObject*>*> layers;	// Layers of GameObjects in this space. layers[0] if bottom, layers[size-1] is top
	short numLayers;									// Number of layers of GameObjects contained in this space

	public:
		BCE_Space(short numLayers);	// Constructor

		void freeMemory();			// Free memory allocated by this class

		bool addGameObject(BCE_GameObject* gameObject, short layerNum);						// Add a GameObject to this space
		bool detectCollision(BCE_GameObject* mobileObject, BCE_GameObject* staticObject);	// Detect if 2 GameObjects are colliding
			
		std::vector<BCE_GameObject*> getLayer(short layerNum);								// Get a layer vector
		short getNumLayers();																// Get number of layers

};