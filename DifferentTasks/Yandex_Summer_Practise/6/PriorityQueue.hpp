/*
 * =====================================================================================
 *
 *       Filename:  PriorityQueue.hpp
 *
 *    Description:  Contain class PriorityQueue, which represents usual priority queue, 
                    as known as std::prioity_queue.
 *
 *        Version:  1.0
 *        Created:  26.04.2014 09:19:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include "BinaryHeap.hpp"
#include "BinominalHeap.hpp"

//as templates you can pass type of value, type of priority key, Comparator and container
template <  class TValue, class TPriority = long long, class Compare = std::less<TPriority>,
            class Container = BinaryHeap<TValue, TPriority, Compare> >
class PriorityQueue {
public:
    
    PriorityQueue() {}
    
    PriorityQueue(const PriorityQueue& another) {
        container = another.container;
    }

    PriorityQueue& operator=(const PriorityQueue& another) {
        if (this != &another) {
            container = another.container;
        }
        return *this;
    }
    
    ~PriorityQueue() {}
    
    bool Empty() {
        return container.Empty();
    }

    size_t Size() {
        return container.Size();
    }
    
    //Push elements in PQ. return id of element. This id can be used in UpdateKey, to change key
    long long Push(const TValue& val, TPriority priority) {
        return container.Push(val, priority);
    }

    const TValue& Top() {
        return container.Top();
    }

    void Pop() {
        container.Pop();
    }
    
    //updating key of element. Take id of element, which was returned by Push. 
    //NOTE: if PQ is minimizing, newPriority should be less than previous nd vice versa
    void UpdateKey(long long id, TPriority newPriority) {
        container.UpdateKey(id, newPriority);
    }

private:
    Container container;
};
