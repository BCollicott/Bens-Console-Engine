#pragma once
#include <vector>
#include "BCGL_GameObject.h"

#define NO_COLLISION {0, 0, -1, -1}	// Returned by detectCollision when its parameters are not overlapping

// Y-Up space containing GameObjects in 1 or more layers
class BCGL_Space {
	std::vector<std::vector<BCGL_GameObject*>*> layers;	// Layers of GameObjects in this space. layers[0] if bottom, layers[size-1] is top
	short numLayers;									// Number of layers of GameObjects contained in this space

	public:
		BCGL_Space(short numLayers);	// Constructor

		void freeMemory();			// Free memory allocated by this class

		bool addGameObject(BCGL_GameObject* gameObject, short layerNum);						// Add a GameObject to this space
		bool detectCollision(BCGL_GameObject* mobileObject, BCGL_GameObject* staticObject);	// Detect if 2 GameObjects are colliding
			
		std::vector<BCGL_GameObject*> getLayer(short layerNum);								// Get a layer vector
		short getNumLayers();																// Get number of layers

};