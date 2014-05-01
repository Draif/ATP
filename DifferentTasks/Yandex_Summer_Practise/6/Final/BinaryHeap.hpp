/*
 * =====================================================================================
 *
 *       Filename:  BinaryHeap.hpp
 *
 *    Description:  Container for PriorityQueue. This container is usual Binary heap.
 *                  More information at http://en.wikipedia.org/wiki/Binary_heap or                   
 *                  in T.H. Cormen, ... "Introduction to algorithms".
 *
 *        Version:  1.0
 *        Created:  26.04.2014 10:00:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include "IPriorityQueue.hpp"
#include <unordered_map>
#include "PriorityQueueException.hpp"
#include <vector>
#include <boost/lexical_cast.hpp>

template <class TValue, class TPriority, class Compare>
class BinaryHeap: public IPriorityQueue<TValue, TPriority> {
public:
	BinaryHeap() : count(0), size(0) {}
    
    BinaryHeap(const BinaryHeap& another) {
        count = another.count;
        size = another.size;
        array = another.array;
        pointersArray = another.pointersArray;
    }
    
    BinaryHeap& operator=(const BinaryHeap& another) {
        if (this != &another) {
            count = another.count;
            size = another.size;
            array = another.array;
            pointersArray = another.pointersArray;
        }
    }
    
    ~BinaryHeap() {}
    
    // true if heap is empty
    bool Empty() const {
        return size == 0;
    }
    // returns size of heap
    size_t Size() const {
        return size;
    }
    
    //returns top element
    const TValue& Top() const {
		if (Empty()) 
			throw PriorityQueueException("PriorityQueue is empty!");
        return array.front().GetValue();
	}

    //extract top element
	void Pop() {
		if (Empty()) 
			throw PriorityQueueException("PriorityQueue is empty!");
		
        size_t deletedPosition = array.front().GetPosition();
		array.front() = array.back();
		pointersArray[array.back().GetPosition()] = 0;
		array.pop_back();
		
        Heapify(0);
        pointersArray.erase(deletedPosition);
		
        --size;
    }

    //allow update key for element already in heap.
	void UpdateKey(long long positionOfNode, TPriority newKey) {
        if (pointersArray.count(positionOfNode) == 0)
            throw PriorityQueueException("No such node in Priority Queue with id = " + 
                                        boost::lexical_cast<std::string>(positionOfNode) + " !");
        if (!comp(newKey, array[pointersArray.at(positionOfNode)].GetPriority()))
            throw PriorityQueueException("Compare return false! Old key: " +
                                        boost::lexical_cast<std::string>(array[pointersArray.at(positionOfNode)].GetPriority()) +
                                        " New key: " + boost::lexical_cast<std::string>(newKey));
        
        array[pointersArray.at(positionOfNode)].SetPriority(newKey);
		
        UpdateKeyHelp(pointersArray.at(positionOfNode));
	}

    //Push elemet in heap. Return unique indificator of new element, which can be used in UpdateKey.
	long long Push(const TValue& val, TPriority priority) {
		array.push_back(BasicHeapNode<TValue, TPriority>(val, priority));
		array.back().SetPosition(count);
		pointersArray.emplace(count, array.size() - 1);
		array.back().SetPosition(count);
		
        UpdateKeyHelp(array.size() - 1);
		
        ++count;
        ++size;
		
        return count - 1;
	}

private:
    
    // Shift up of element. Used in Push.
	void UpdateKeyHelp(size_t i) {
		while (i > 0 && !comp(array[Parent(i)].GetPriority(), array[i].GetPriority())){
			std::swap(pointersArray.at(array[i].GetPosition()), pointersArray.at(array[Parent(i)].GetPosition()));
			std::swap(array[i], array[Parent(i)]);
			i = Parent(i);
		}
	}

    //three functions, which returns indexes
	size_t Parent(size_t i){
		return (i - 1)/2;
	}
	size_t LeftChild(size_t i){
		return 2*i + 1;
	}
	size_t RightChild(size_t i){
		return 2*i + 2;
	}

    //Shift down. Used in Pop.
	void Heapify(unsigned int i){
		size_t l = LeftChild(i);
		size_t r = RightChild(i);
		size_t largest;
    
		if ( l < array.size() && comp(array[l].GetPriority(), array[i].GetPriority()))
			largest = l;
		else
			largest = i;
		
        if (r < array.size() && comp(array[r].GetPriority(), array[largest].GetPriority()))
			largest = r;
		
        if (largest != i) {
			std::swap(pointersArray.at(array[i].GetPosition()), pointersArray.at(array[largest].GetPosition()));
			std::swap(array[i], array[largest]);
			Heapify(largest);
		}
	}

	long long  count;
    size_t size;
    
    Compare comp; // object of comparator
	std::vector<BasicHeapNode<TValue, TPriority>> array; //contains heap itself
    
    //map unique index of element(returned by Push) and its place in array. Used in UpdateKey 
    std::unordered_map<long long, size_t> pointersArray;
};


