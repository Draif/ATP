/*
 * =====================================================================================
 *
 *       Filename:  BinominalHeap.hpp
 *
 *    Description:  Container for PriorityQueue. This container is usual Binominal heap.
 *                  More information at http://en.wikipedia.org/wiki/Binominal_heap or                   
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
#include <boost/lexical_cast.hpp>

template <class TValue, class TPriority, class Compare>
class BinominalHeap : public IPriorityQueue<TValue, TPriority> {
    
    class BasicBinominalHeapNode: public BasicHeapNode<TValue, TPriority> {
        typedef BasicBinominalHeapNode ptr;
    public:
        BasicBinominalHeapNode(const TValue& _val, TPriority prior) : BasicHeapNode<TValue, TPriority>(_val, prior), degree(0) {}
        
        void SetParentPtr(std::shared_ptr<ptr> _parent) {
            parent = _parent;
        }
        
        void SetChildPtr(std::shared_ptr<ptr> _child) {
            child = _child;
        }
        
        void SetRightSiblingPtr(std::shared_ptr<ptr> _rightSibling) {
            rightSibling = _rightSibling;
        }

        void SetDegree(int _degree) {
            degree = _degree;
        }
        
        int GetDegree() const {
            return degree;
        }

        std::shared_ptr<ptr> GetRightSiblingPtr()const {
            return rightSibling;
        }
        
        std::shared_ptr<ptr> GetParentPtr() const {
            return parent.lock();
        }
        
        std::shared_ptr<ptr> GetChildPtr() const {
            return child;
        }
    
    private:
        std::shared_ptr<ptr> rightSibling;
        std::weak_ptr<ptr> parent;
        std::shared_ptr<ptr> child;
        int degree; 
    };

    typedef BinominalHeap<TValue, TPriority, Compare>::BasicBinominalHeapNode node;

public:
	
    BinominalHeap() : count(0), size(0) {}
    
    BinominalHeap(const BinominalHeap& another) {
        head = another.head;
        count = another.count;
        size = another.size;
        pointersToNodes = another.pointersToNodes;
    }
    
    BinominalHeap& operator=(const BinominalHeap& another) {
        if (this != &another) {
            head = another.head;
            count = another.count;
            size = another.size;
            pointersToNodes = another.pointersToNodes;
        }
    }
    
    ~BinominalHeap() {}
    
    //Pop element from heap
	void Pop();
    //Change key for element in heap. Uses unique id, got from Push
	void UpdateKey(long long, TPriority);
    
    //true if empty
    bool Empty() const {
        return size == 0;
    }

    //returns size
    size_t Size() const {
        return size;
    }
    
    //returns top element
    const TValue& Top() const {
		if (Empty()) 
			throw PriorityQueueException("PriorityQueue  is empty!");
        
        std::shared_ptr<node> x = head;
		std::shared_ptr<node> y = x;
        long long _min = x->GetPriority(); 
        
        while (x) {
			if (comp(x->GetPriority(), _min)) {
				_min = x->GetPriority();
				y = x;
			}
		
            std::shared_ptr<node> temp = x->GetRightSiblingPtr();
			x = temp;
		}
        
        return y->GetValue();
	}

    //Push element. Return unique id of element.
	long long Push(const TValue& _node, TPriority priority) {
		std::shared_ptr<node> newNode (new node(_node, priority));
		newNode->SetPosition(count);
		pointersToNodes.emplace(count, newNode);
        BinominalHeapUnion(newNode, head);
		++count;
        ++size;
		return count - 1;
	}

private:

    //helper function, used in HeapUnion
	void ReverseChildren (std::shared_ptr<node>, std::shared_ptr<node>, std::shared_ptr<node>&);
    
    //helper function, used in HeapUnion
    std::shared_ptr<node> BinominalHeapMerge(std::shared_ptr<node>, std::shared_ptr<node>); 
	
    //helper function, in Push, Pop 
    void BinominalHeapUnion(std::shared_ptr<node>, std::shared_ptr<node>);
	
    //Helper function, used in HeapUnion
    void BinominalLink(std::shared_ptr<node>& y, std::shared_ptr<node>& z) {
		y->SetParentPtr(z);
		y->SetRightSiblingPtr(z->GetChildPtr());
		z->SetChildPtr(y);
		z->SetDegree(z->GetDegree() + 1);
	}
    
    Compare comp;//object of comparator
	std::shared_ptr<node> head; // pointer to head of heap
	std::unordered_map<long long, std::shared_ptr<node>> pointersToNodes; //map node id and pointer to it
	long long count; // 
    int size;
};

//Below are declaration of member-function, defined in class BinominalHeap.
template <class TValue, class TPriority, class Compare>
void BinominalHeap<TValue, TPriority, Compare>::Pop() {
    if (Empty())
        throw PriorityQueueException("Priority Queue is empty!");
    std::shared_ptr<node> x = head;
    std::shared_ptr<node> y = x;
    std::shared_ptr<node> prev;
    std::shared_ptr<node> previous;
    std::shared_ptr<node> children;
    TPriority _min = x->GetPriority();

    while (x) {
        if (comp(x->GetPriority(), _min)) {
            _min = x->GetPriority();
            previous = std::move(prev);
            y = x;
        }
        std::shared_ptr<node> temp = x->GetRightSiblingPtr();
        prev = std::move(x);
        x = temp;
    }
    
    if (!previous) {
        head = y->GetRightSiblingPtr();
    } else {
        previous->SetRightSiblingPtr(y->GetRightSiblingPtr());
    }

    if (y->GetChildPtr()) {
        if (y->GetChildPtr()->GetRightSiblingPtr()) {
            ReverseChildren(y->GetChildPtr(), y->GetChildPtr()->GetRightSiblingPtr(), children);
        } else {
            children = y->GetChildPtr();
            y->GetChildPtr()->SetParentPtr(nullptr);
        }
        BinominalHeapUnion(head, children);
    }

    pointersToNodes.erase(y->GetPosition());
    --size;
}

template <class TValue, class TPriority, class Compare>
void BinominalHeap<TValue, TPriority, Compare>::UpdateKey(long long nodeNumber, TPriority newKey) {
    if (pointersToNodes.count(nodeNumber) == 0)
        throw PriorityQueueException("No such node in Priority Queue with id = " + 
                                    boost::lexical_cast<std::string>(nodeNumber) + " !");
    if (!comp(newKey, pointersToNodes.at(nodeNumber)->GetPriority()))
        throw PriorityQueueException("Compare return false! Old key: " +
                                        boost::lexical_cast<std::string>(pointersToNodes.at(nodeNumber)->GetPriority()) +
                                        " New key: " + boost::lexical_cast<std::string>(newKey));
    pointersToNodes.at(nodeNumber)->SetPriority(newKey);
    if (pointersToNodes.at(nodeNumber)->GetParentPtr()) {
        int parent = pointersToNodes.at(nodeNumber)->GetParentPtr()->GetPosition();
        
        while (parent >= 0 && !comp(pointersToNodes.at(parent)->GetPriority(), pointersToNodes.at(nodeNumber)->GetPriority())) {
            int tempParent = -1;
            if (pointersToNodes.at(parent)->GetParentPtr())
                tempParent = pointersToNodes.at(parent)->GetParentPtr()->GetPosition();
            std::shared_ptr<node> temp = pointersToNodes.at(parent);
            pointersToNodes.at(nodeNumber)->swapContent(*pointersToNodes.at(parent));
            pointersToNodes.at(nodeNumber)->SetPosition(parent);
            pointersToNodes.at(parent)->SetPosition(nodeNumber);
            pointersToNodes.at(parent) = pointersToNodes.at(nodeNumber);
            pointersToNodes.at(nodeNumber) = temp;
            parent = tempParent;
        }
    }
}

template <class TValue, class TPriority, class Compare>
void BinominalHeap<TValue, TPriority, Compare>::ReverseChildren (std::shared_ptr<node> first, std::shared_ptr<node> second, std::shared_ptr<node>& children) {
    if (!second->GetRightSiblingPtr()) {
        first->SetRightSiblingPtr(nullptr);
        second->SetRightSiblingPtr(first);
        first->SetParentPtr(nullptr);
        second->SetParentPtr(nullptr);
        children = second;
    } else {
        ReverseChildren(second, second->GetRightSiblingPtr(), children);
        first->SetRightSiblingPtr(nullptr);
        second->SetRightSiblingPtr(first);
        second->SetParentPtr(nullptr);
        first->SetParentPtr(nullptr);
    }
}

template <class TValue, class TPriority, class Compare>
std::shared_ptr<typename BinominalHeap<TValue, TPriority, Compare>::BasicBinominalHeapNode> 
    BinominalHeap<TValue, TPriority, Compare>::BinominalHeapMerge(std::shared_ptr<node> first, std::shared_ptr<node> second) {
    
    std::shared_ptr<node> finalResult;
    std::shared_ptr<node> temp;

    if (first)
        finalResult = first;
    else 
        finalResult = second;

    for (int i = 0; first && second; ++i) {
        if (first->GetDegree() <= second->GetDegree()) {
            if (i == 0) {
                finalResult = first;
            }
            temp = first;
            std::shared_ptr<node> _temp = first->GetRightSiblingPtr();
            first = _temp;
        } else {
            if (i == 0) {
                finalResult = second;
            }
            if (temp)
                temp->SetRightSiblingPtr(second);
            std::shared_ptr<node> _temp = second->GetRightSiblingPtr();
            second->SetRightSiblingPtr(first);
            temp = second;	
            second = _temp;
        }
    }

    if (!first && temp)
        temp->SetRightSiblingPtr(second);

    return finalResult;
}

template <class TValue, class TPriority,  class Compare>
void BinominalHeap<TValue, TPriority, Compare>::BinominalHeapUnion(std::shared_ptr<node> first, std::shared_ptr<node> second) {
    head = std::move(BinominalHeapMerge(first, second));
    
    if (!head)
        return;
    
    std::shared_ptr<node> prev_x;
    std::shared_ptr<node> x = head;
    std::shared_ptr<node> next_x = x->GetRightSiblingPtr();
    
    while (next_x) {
        if (x->GetDegree() != next_x->GetDegree() 
            || (next_x->GetRightSiblingPtr() && next_x->GetRightSiblingPtr()->GetDegree() == x->GetDegree())) {
                prev_x = std::move(x);
                x = std::move(next_x);
        } else {

            if (comp(x->GetPriority(), next_x->GetPriority())) {
                x->SetRightSiblingPtr(next_x->GetRightSiblingPtr());
                BinominalLink(next_x, x);
            } else {

                if (!prev_x) {
                    head = next_x;
                } else {
                    prev_x->SetRightSiblingPtr(next_x);
                }

                BinominalLink(x, next_x);
                x = std::move(next_x);
            }
        }
        next_x = x->GetRightSiblingPtr();
    }
}


