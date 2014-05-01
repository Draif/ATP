#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include "PriorityQueue.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <exception>

//class for tests
class TestNode {
    int value;
    int priority;
public:
    TestNode(int val, int prio) : value(val), priority(prio) {}
    bool operator<(const TestNode& another) {
        return priority < another.priority;
    }
    bool operator==(const TestNode& another) {
        return value == another.value;
    }
};

//basic test for Priority Queue. Testing Empty(), Size(). 
//Also pushing and then poping elements from Priority Queue.
template <typename Heap>
void Test1(int numberOfNodes) {
    Heap heap;
    BOOST_CHECK(heap.Empty()); //PQ should be empty
    
    //generator for random values
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);  
    int randomValue;
    
    std::vector<int> vec;//vector to contain random values
    
    for (int i = 0; i < numberOfNodes; ++i) {
        randomValue = generator();//generate random numbers
        vec.push_back(randomValue);
        heap.Push(randomValue, randomValue);
    }
    
    BOOST_CHECK_EQUAL(heap.Size(), numberOfNodes);//check size
    
    std::sort(vec.begin(), vec.end());//sort elements in vector
    
    for (int i = 0; !heap.Empty(); ++i) {
        BOOST_CHECK_EQUAL(heap.Top(), vec[i]);
        heap.Pop();
    }
    BOOST_CHECK(heap.Empty());
    
    // check for good work of destructor. I also check it with valgrind 
    for (int i = 0; i < numberOfNodes; ++i) {
        randomValue = generator();//generate random numbers
        heap.Push(randomValue, randomValue);
    }
}

//test for maximizing Priority Queue.
//Generate random values, push and then pop them.
template <typename Heap>
void Test2(int numberOfNodes) {
    Heap heap;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);  
    int randomValue;
    std::vector<int> vec;
    for (int i = 0; i < numberOfNodes; ++i) {
        randomValue = generator();
        vec.push_back(randomValue);
        heap.Push(randomValue, randomValue);
    }
    BOOST_CHECK_EQUAL(heap.Size(), numberOfNodes);
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    for (int i = 0; !heap.Empty(); ++i) {
        BOOST_CHECK_EQUAL(heap.Top(), vec[i]);
        heap.Pop();
    }
}    

//Push elements, them pop some of them, after that push them again in PQ
template <typename Heap>
void Test3(int numberOfNodes) {
    Heap heap;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);  
    int randomValue;
    std::vector<int> vec;
    for (int i = 0; i < numberOfNodes; ++i) {
        randomValue = generator();
        vec.push_back(randomValue);
        heap.Push(randomValue, randomValue);
    }
    std::sort(vec.begin(), vec.end());
    int deletedNodes = numberOfNodes / 2;
    for (int i = 0; i < deletedNodes; ++i)
        heap.Pop();
    
    for (int i = 0; i < deletedNodes; ++i)
        heap.Push(vec[i], vec[i]);

    BOOST_CHECK_EQUAL(heap.Size(), numberOfNodes);
    for (int i = 0; !heap.Empty(); ++i) {
        BOOST_CHECK_EQUAL(heap.Top(), vec[i]);
        heap.Pop();
    }
}

//test for throwing exception
template <typename Heap>
void Test4() {
    Heap heap;
    BOOST_CHECK_THROW(heap.Pop(), PriorityQueueException);//heap is empty
    long long id = heap.Push(1,1);
    BOOST_CHECK_THROW(heap.UpdateKey(id + 1, 1), PriorityQueueException);//try update node with wrong id
    BOOST_CHECK_THROW(heap.UpdateKey(id, 2), PriorityQueueException);//try maximize key in minimizing PQ
}
//test for UpdateKey. Push elements with greater priority, then decrease key and check that everything is OK
template <typename Heap>
void Test5(int numberOfNodes) {
    Heap heap;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);  
    int addedValue = 1000;
    long long randomValue;
    std::vector<long long> vec;
    std::vector<long long>  nodesId;//save nodes id, returning from Push
    for (int i = 0; i < numberOfNodes; ++i) {
        randomValue = generator() + addedValue;
        vec.push_back(randomValue);
        nodesId.push_back(heap.Push(randomValue, randomValue));
    }
    for (auto p = nodesId.begin(), v = vec.begin(); p != nodesId.end(); ++p, ++v)
        heap.UpdateKey(*p, *v - addedValue);//decrease priority

    std::sort(vec.begin(), vec.end());
    BOOST_CHECK_EQUAL(heap.Size(), numberOfNodes);
    for (int i = 0; !heap.Empty(); ++i) {
        BOOST_CHECK_EQUAL(heap.Top(), vec[i]);
        heap.Pop();
    }
}

BOOST_AUTO_TEST_CASE(Binary_Heap) {
	Test1<PriorityQueue<int>>(10000);
	Test2<PriorityQueue<int, long long, std::greater<long long>>>(10000);
	Test3<PriorityQueue<int>>(10000);
	Test4<PriorityQueue<int>>();
	Test5<PriorityQueue<long long, long long>>(10000);
	Test1<PriorityQueue<int>>(1000000); // stess test
}

BOOST_AUTO_TEST_CASE(Binominal_Heap) {
	Test1<PriorityQueue<int, long long, std::less<long long>, BinominalHeap<int, long long, std::less<long long>>>>(10000);
	Test2<PriorityQueue<int, long long, std::greater<long long>, BinominalHeap<int, long long, std::greater<long long>>>>(10000);
	Test3<PriorityQueue<int, long long, std::less<long long>, BinominalHeap<int, long long, std::less<long long>>>>(10000);
	Test4<PriorityQueue<int, long long, std::less<long long>, BinominalHeap<int, long long, std::less<long long>>>>();
	Test5<PriorityQueue<long long,  long long, std::less<long long>, BinominalHeap<long long, long long, std::less<long long>>>>(10000);
	Test1<PriorityQueue<int, long long, std::less<long long>, BinominalHeap<int, long long, std::less<long long>>>>(1000000); //stress test 
}

