#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\DataStructure\BinaryTree.h"
#include "..\DataStructure\DoublyLinkedList.h"
#include "..\DataStructure\Pair.h"
#include "..\DataStructure\Stack.h"
#include "..\DataStructure\Queue.h"
#include "..\DataStructure\Map.h"
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructuretests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(BinarySearchTree_TEST)
		{
			BinaryTree< int > tree;
			Assert::IsTrue(tree.insert(2) == Pair< int, bool >::make_pair(2, true));
			Assert::IsTrue(tree.insert(4) == Pair< int, bool >::make_pair(4, true));
			Assert::IsTrue(tree.insert(4) == Pair< int, bool >::make_pair(4, false));
			Assert::AreEqual(2, tree.size());
			Assert::IsTrue(nullptr == tree.find(3));
			Assert::IsTrue(2 == *(tree.find(2)));
			Assert::IsFalse (nullptr == tree.find(4));
			Assert::IsFalse(tree.remove(3));
			Assert::IsTrue(tree.remove(4));
			Assert::AreEqual(1, tree.size());
			Assert::IsTrue(tree.insert(4) == Pair< int, bool >::make_pair(4, true));
			Assert::IsTrue(2 == tree.size());
			tree.insert(20);
			tree.insert(25);
			tree.insert(13);
			tree.insert(92);
			tree.insert(3);
			Assert::IsTrue(7 == tree.size());
			tree.remove(4);
			Assert::IsTrue(3 == *(tree.find(3)));
			tree.clear();
			Assert::AreEqual(0, tree.size());
		}

		TEST_METHOD(DoublyLinkedList_TEST){
			DoublyLinkedList< int > list;
			list.push_back(2);
			list.push_back(2);
			list.push_front(1);
			list.push_back(3);
			list.push_back(100);

			// 1 2 2 3 100
			Assert::AreEqual(5, list.size());
			Assert::AreEqual(100, list.back());
			Assert::AreEqual(1, list.front());
			

			list.pop_back();
			list.pop_front();
			
			// 2 2 3
			Assert::AreEqual(3, list.size());
			Assert::AreEqual(2, list.front());
			Assert::AreEqual(3, list.back());

			list.push_back(4);
			list.push_front(2);
			
			// 2 2 2 4
			Assert::AreEqual(4, list.back());
			list.clear();
			Assert::AreEqual(0, list.size());

			list.push_back(4);
			list.push_front(2);
			// 2 4
			Assert::AreEqual(2, list.front());
		}

		TEST_METHOD(Stack_TEST){
			Stack< int > stack;
			stack.push(4);
			stack.push(1);
			stack.push(3);

			Assert::AreEqual(3, stack.size());
			Assert::AreEqual(3, stack.top());
			Assert::AreEqual(3, stack.pop());
			Assert::AreEqual(1, stack.pop());
			Assert::AreEqual(4, stack.pop());
			Assert::AreEqual(0, stack.size());
			
			stack.clear();
			stack.push(5);
			stack.push(19);
			stack.push(6);
			Assert::AreEqual(6, stack.top());
		}

		TEST_METHOD(Queue_TEST){
			Queue< int > queue;
			int i = 0;
			int list[] = { 1, 4, 3, 2, 5, 3 };

			while (++i < 10){
				for (int i = 0; i < sizeof(list) / sizeof(int); ++i)
					queue.push_back(list[i]);


				Assert::AreEqual(6, queue.size());
				for (int i = 0; i < sizeof(list) / sizeof(int); ++i){
					Assert::AreEqual(list[i], queue.front());
					queue.pop_front();
				}
				queue.clear();
				Assert::AreEqual(0, queue.size());
			}
			queue.clear();
			Assert::AreEqual(0, queue.size());
		}

		TEST_METHOD(Map_TEST){
			Map< int, int > m;
			m.insert(1, 2);
			m.insert(Pair< int, int >::make_pair(2, 3));

			Assert::AreEqual(2, m[1]);
			m.insert(1, 3);
			Assert::AreEqual(3, m[1]);
			Assert::AreNotEqual(2, m[1]);
			Assert::AreEqual(3, m[2]);
			Assert::AreEqual(2, m.size());

			m.clear();
			Assert::AreEqual(0, m.size());
		}

	};
}