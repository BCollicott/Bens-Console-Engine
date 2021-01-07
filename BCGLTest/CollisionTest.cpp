#include "pch.h"
#include "CppUnitTest.h"

#include <Windows.h>
#include "BCGL_Space.h"
#include "BCGL_GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CollisionTest
{
	TEST_CLASS(CollisionTest)
	{
	public:
		// Objects used for testing
		BCGL_Space* space;
		BCGL_GameObject* mobileObject;
		BCGL_GameObject* staticObject;

		void init()
		{
			space = new BCGL_Space(1);
			mobileObject = new BCGL_GameObject({ 0, 0 }, { 4, 2 }, nullptr);
			staticObject = new BCGL_GameObject({ 0, 0 }, { 4, 4 }, nullptr);
			space->addGameObject(mobileObject, 0);
			space->addGameObject(staticObject, 0);
		}

		void free()
		{
			space->freeMemory();
			delete(space);
			delete(mobileObject);
			delete(staticObject);
		}

		// GameObjects collide in oroginal position
		TEST_METHOD(TestCollision1)
		{
			init();
			Assert::AreEqual(true, space->detectCollision(mobileObject, staticObject));
			free();
		}
		
		// GameObjects do not collide in disjoint positions
		TEST_METHOD(TestCollision2)
		{
			init();
			mobileObject->setPos({ -4, 0 });
			Assert::AreEqual(false, space->detectCollision(mobileObject, staticObject));
			free();
		}
	};
}