/*
 * =====================================================================================
 *
 *       Filename:  IPriorityQueue.hpp
 *
 *    Description:  This file contains the interaface for containers, 
 *                  which can be used in PriorityQueue.
 *                  Also there is class "BasicHeapNode", which is used directly
 *                  in containers or as base.
 *
 *        Version:  1.0
 *        Created:  26.04.2014 09:10:49
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com, 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once

template <class TValue, class TPriority> 
class IPriorityQueue {
public:
    virtual bool Empty() const = 0;

    virtual size_t Size() const = 0;
    
    virtual long long Push(const TValue&, TPriority priority) = 0;
	
    virtual const TValue& Top() const = 0;

    virtual void Pop() = 0;
	
    virtual void UpdateKey(long long, TPriority) = 0;
};

template <class TValue, class TPriority>
class BasicHeapNode {
public:
    BasicHeapNode(const TValue& _val, TPriority pr) : val(_val), priority(pr), position(0) {}
    
    BasicHeapNode(const BasicHeapNode& another) {
        priority = another.priority;
        val = another.val;
        position = another.position;
    }
    
    BasicHeapNode& operator=(const BasicHeapNode& another) {
        if (this != &another) {
            priority = another.priority;
            val = another.val;
            position = another.position;
        }
        return *this;
    }
    // set and get position of node. Position used in UpdateKey 
    long long GetPosition() const {
        return position;
    }
    
    void SetPosition(long long pos) {
        position = pos;
    }

	TPriority GetPriority() const {
		return priority;
	}	
	
    void SetPriority(TPriority _priority) {
		priority = _priority;
	}
    
    const TValue& GetValue() const {
        return val;
    }
    
    void swapContent(BasicHeapNode& another) {
        std::swap(val, another.val);
    }
private:
	TValue val;
    TPriority priority;
	long long position;
};



