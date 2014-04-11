#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\DataStructure\AVL.h"
#include "..\DataStructure\Pair.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructuretests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(AVL_TEST)
		{
			AVL< int > tree;
			Assert::IsTrue(tree.insert(2) == Pair< int, bool >::make_pair(2, true));
			Assert::IsTrue(tree.insert(4) == Pair< int, bool >::make_pair(4, true));
			Assert::IsTrue(tree.insert(4) == Pair< int, bool >::make_pair(4, false));

			Assert::AreEqual(2, tree.size());
			Assert::IsTrue(nullptr == tree.find(3));
			Assert::IsTrue(2 == *(tree.find(2)));
			Assert::IsFalse (nullptr == tree.find(4));
			tree.erase();
			Assert::AreEqual(0, tree.size());
		}

	};
}