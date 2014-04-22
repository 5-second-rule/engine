#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\engine-core\World.h"
#include "..\engine-core\WorldObject.h"
#include "..\engine-core\ConfigSettings.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

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
			WorldObject* a;
			WorldObject* b;
			a = new WorldObject();
			b = new WorldObject();
			WorldObjectHandle ha(a);
			WorldObjectHandle hb(b);

			Assert::IsTrue(a == ha.toObject());
			Assert::IsTrue(b == hb.toObject());
			Assert::AreEqual(2, World::getObjectCounter());
			delete(b);
			b = new WorldObject();
			Assert::AreEqual(2, World::getObjectCounter());
			Assert::IsTrue(nullptr == hb.toObject());	
		}

		*/

		TEST_METHOD(TestConfiguration){
			int testInt;
			double testDouble;
			float testFloat;
			bool testBool;
			string testString;

			Assert::IsTrue(ConfigSettings::config->checkIfLoaded());
			ConfigSettings::config->getValue("TestConfig#int", testInt);
			ConfigSettings::config->getValue("TestConfig#float", testFloat);
			ConfigSettings::config->getValue("TestConfig#double", testDouble);
			ConfigSettings::config->getValue("TestConfig#bool", testBool);
			ConfigSettings::config->getValue("TestConfig#string", testString);

			Assert::AreEqual(1004, testInt);
			Assert::AreEqual(true, testBool);
			Assert::AreEqual((float)200.1, testFloat);
			Assert::AreEqual(200.1, testDouble);
			Assert::AreEqual("bife", testString.c_str());

			ConfigSettings::config->updateValue("TestConfig#newValue", 24);
			ConfigSettings::config->saveSettingsFile();

			ConfigSettings::config->reloadSettingsFile();
			
			Assert::IsTrue(ConfigSettings::config->checkIfLoaded());
			ConfigSettings::config->getValue("TestConfig#newValue", testInt);
			Assert::AreEqual(24, testInt);
		}
	};
}