#include "pch.h"
#include "CppUnitTest.h"

#include <Windows.h>
#include "BCE_Space.h"
#include "BCE_GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CollisionTest
{
	TEST_CLASS(CollisionTest)
	{
	public:
		// Objects used for testing
		BCE_Space* space;
		BCE_GameObject* mobileObject;
		BCE_GameObject* staticObject;

		void init()
		{
			space = new BCE_Space(1);
			mobileObject = new BCE_GameObject({ 0, 0 }, { 4, 2 }, nullptr);
			staticObject = new BCE_GameObject({ 0, 0 }, { 4, 4 }, nullptr);
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