#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\engine-core\World.h"
#include "..\engine-core\GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace enginecoretests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		/* TODO reimplement

		TEST_METHOD(TestWorld)
		{
			World* world = World::getInstance();

			Assert::AreEqual(0, world->getObjectCounter());
		}

		TEST_METHOD(TestObjectHandle)
		{
			GameObject* a;
			GameObject* b;
			a = new GameObject();
			b = new GameObject();
			GameObjectHandle ha(a);
			GameObjectHandle hb(b);

			Assert::IsTrue(a == ha.toObject());
			Assert::IsTrue(b == hb.toObject());
			Assert::AreEqual(2, World::getObjectCounter());
			delete(b);
			b = new GameObject();
			Assert::AreEqual(2, World::getObjectCounter());
			Assert::IsTrue(nullptr == hb.toObject());	
		}

		*/


	};
}