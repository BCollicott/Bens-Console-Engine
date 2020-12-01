#pragma once
#include <vector>
#include "BCE_GameObject.h"

// Y-Up space containing GameObjects
class BCE_Space {
	public:
		std::vector<BCE_GameObject*> gameObjects;	// GameObjects in this space
		void addGameObject(BCE_GameObject* gameObject);
};