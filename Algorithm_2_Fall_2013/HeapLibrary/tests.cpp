#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include "HeapLibrary.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>


class TestNode: public BasicFibonacciHeapNode<TestNode> {
	int id;
public:
	TestNode() {
	}

	TestNode(int i): id(i) {
	}

	TestNode(int i, long long _priority): id(i) {
		SetPriority(_priority);
	}

	TestNode(const TestNode& another) {
		*this = another;
	}

	bool operator== (const TestNode& another) const {
		return id == another.id;
	}

	const int GetID() const {
		return id;
	}

	TestNode& operator=(const TestNode& another) {
		SetPriority(another.GetPriority());
		id = another.id;
		SetChildPtr(another.GetChildPtr());
		SetParentPtr(another.GetParentPtr());		
		SetRightSiblingPtr(another.GetRightSiblingPtr());
		SetLeftSiblingPtr(another.GetLeftSiblingPtr());
		position = another.position;
		return *this;
	}

	virtual void swapContent (TestNode& another) override {
		long long tempPriority = GetPriority();
		int tempId = id;
		SetPriority(another.GetPriority());
		another.SetPriority(tempPriority);
		id = another.id;
		another.id = tempId;
	}

	bool operator<(const TestNode& another) const {
		return GetPriority() < another.GetPriority();
	}
};

std:: ostream& operator<<(std::ostream& out, const TestNode& num){
	std:: cout << "Node\t id = " << num.GetID() << "\t priority = " << num.GetPriority() << std::endl;	
	return out;
}

template <typename Heap> 
void basicTest() {
	Heap heap;
	const int numberOfNodes = 1000;
	std:: vector<TestNode> vec;
	std::vector<int> positions(10);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);  
	for (int i = 0; i < numberOfNodes; ++i) {
		long long temp = generator();
		vec.push_back(TestNode(temp, temp));
		heap.Add(vec[i]);
	}
	BOOST_CHECK_EQUAL(heap.GetMin(), vec.front());
	for (int i = 0; i < numberOfNodes; ++i){
        std::cout << "First check " << i << std::endl;
		BOOST_CHECK_EQUAL(heap.GetMin(), vec[i]);
		TestNode temp = heap.ExtractMin();
		//std::cout << temp;
		BOOST_CHECK_EQUAL(temp, vec[i]);
	}
	for (int i = 0; i < numberOfNodes; ++i) {
		long long temp = i + 10;
		vec[i].SetPriority(temp);
		positions[i] = heap.Add(vec[i]);
	}
	for (int i = 0; i < numberOfNodes; ++i) {
		heap.UpdateKey(positions[i], 9 - i);
		vec[i].SetPriority(9 - i);
	}
	BOOST_CHECK_EQUAL(heap.GetMin(),vec.back());
	for (int i = 9; i >= 0; --i){
		BOOST_CHECK_EQUAL(heap.GetMin(), vec[i]);
		TestNode temp = heap.ExtractMin();
		//std::cout << temp;
		BOOST_CHECK_EQUAL(temp, vec[i]);
	}
}


template <typename Heap> 
void advancedTest() {
	Heap heap;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);  // minstd_rand0 is a standard linear_congruential_engine
	const int numberOfNodes = 100;
	srand(time(0));
	std::vector<TestNode> vec (100);
    std::vector<int> indx;
	for (int i = 0; i < numberOfNodes; ++i) {
		vec[i] = TestNode(i, generator());
		indx.push_back(heap.Add(vec[i]));
	}
	std::sort(vec.begin(), vec.end());
	for (int i = 0; i < numberOfNodes; ++i){
		BOOST_CHECK_EQUAL(heap.GetMin(), vec[i]);
		TestNode temp = heap.ExtractMin();
	BOOST_CHECK_EQUAL(temp, vec[i]);
	}

}

/*
BOOST_AUTO_TEST_CASE(my_test) {
	basicTest<BinaryHeap<TestNode>>();
	advancedTest<BinaryHeap<TestNode>>();
}
*/

BOOST_AUTO_TEST_CASE(my_test1) {
	basicTest<BinominalHeap<TestNode>>();
	advancedTest<BinominalHeap<TestNode>>();
}

BOOST_AUTO_TEST_CASE(my_test2) {
	basicTest<FibonacciHeap<TestNode>>();
	advancedTest<FibonacciHeap<TestNode>>();
}





