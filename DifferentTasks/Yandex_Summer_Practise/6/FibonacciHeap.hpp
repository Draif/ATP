/*
 * =====================================================================================
 *
 *       Filename:  FibonacciHeap.hpp
 *
 *    Description:  Container for PriorityQueue. More information at
 *                  http://en.wikipedia.org/wiki/Fibonacci_heap or 
 *                  in T.H. Cormen, ... "Introduction to algorithms".
 *
 *        Version:  1.0
 *        Created:  26.04.2014 10:16:21
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
#include "PriorityQueueException.hpp"
#include <unordered_map>

template <class TValue, class Compare>
class FibonacciHeap : public IPriorityQueue<TValue, TPriority> {
    class BasicFibonacciHeapNode: public BasicHeapNode<TValue> {
        typedef FibonacciHeap<TValue, Compare>::BasicFibonacciHeapNode Parent;
        typedef FibonacciHeap<TValue, Compare>::BasicFibonacciHeapNode ptr;
    public:
        BasicFibonacciHeapNode(const TValue& _val, long long prior) : BasicHeapNode<TValue>(_val, prior), degree(0), mark(false) {}
        
        void SetLeftSiblingPtr(std::shared_ptr<ptr> _parent) {
        	leftSibling.reset();
            leftSibling = std::move(_parent);
        }
        void SetMark(bool _mark) {
            mark = _mark;
        }
        ~BasicFibonacciHeapNode() {
            if (leftSibling.get() == this)
                leftSibling.reset(); 
            if (rightSibling.get() == this)
                rightSibling.reset();	
        }

        std::shared_ptr<ptr> GetLeftSiblingPtr() const {
            return leftSibling;
        }

        bool GetMark() const {
            return mark;
        }
        
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
            return parent;
        }
        std::shared_ptr<ptr> GetChildPtr() const {
            return child;
        }


        virtual void clear() {
            leftSibling.reset();
            rightSibling.reset();
            child.reset();
            parent.reset();
        }

        void swap(std::shared_ptr<ptr> another) {
            std::shared_ptr<ptr> tempParent = GetParentPtr();
            std::shared_ptr<ptr> tempChild = GetChildPtr();
            SetParentPtr(another->GetParentPtr());
            SetChildPtr(another->GetChildPtr());
            another->SetParentPtr(tempParent);
            another->SetChildPtr(tempChild);
            this->swapContent(*another);
        }
    private:
        std::shared_ptr<ptr> leftSibling;
        std::shared_ptr<ptr> rightSibling;
        std::shared_ptr<ptr> parent;
        std::shared_ptr<ptr> child;
        int degree; 
        bool mark;
    };

    typedef BasicFibonacciHeapNode node;
public:
	FibonacciHeap() : count(0), size(0) {}
    bool Empty() const {
        return size == 0;
    }

    size_t Size() const {
        return size;
    }
        
	long long Push(const TValue&, long long);	
    
    const TValue& Top() const {
		return _min->GetValue();
	}

	void Pop();

    void UpdateKey(long long nodeNumber, long long newKey) {
		std::shared_ptr<node> x = pointersToNodes.at(nodeNumber);
		if (!x)
			throw PriorityQueueException("UpdateKey: no such node!");
		if (comp(x->GetPriority(), newKey))
			throw PriorityQueueException("UpdateKey: new priority is higher than old one!");
		x->SetPriority(newKey);
		std::shared_ptr<node> y = x->GetParentPtr();
		if (y && comp(x->GetPriority(), y->GetPriority())) {
			Cut(x, y);
			CascadingCut(y);
		}
		if (comp(x->GetPriority(), _min->GetPriority())) {
			//_min.reset();
			_min = x;
		}
	}

private:

   
	std::shared_ptr<node> FindNode(const node&, std::shared_ptr<node>);
	
    void Cut(std::shared_ptr<node>&, std::shared_ptr<node>&); 
	
    void CascadingCut(std::shared_ptr<node>& y) {
		std::shared_ptr<node> z = y->GetParentPtr();
		if (z) {
			if (!y->GetMark())
				y->SetMark(true);
			else {
				Cut(y, z);
				CascadingCut(z);
			}
		}
	}

	void FibHeapLink(std::shared_ptr<node>, std::shared_ptr<node>); 
	void Consolidate();
    Compare comp;
    std::shared_ptr<node> _min;
	std::unordered_map<int, std::shared_ptr<node>> pointersToNodes;
	int count;
	int size;
};

template <class TValue, class Compare>
long long FibonacciHeap<TValue, Compare>::Push(const TValue& value, long long priority) {
    std::shared_ptr<node> newNode (new node(value, priority));
    //*newNode = _node;
    /*newNode->SetDegree(0);
    newNode->SetParentPtr(nullptr);
    newNode->SetChildPtr(nullptr);
    newNode->SetLeftSiblingPtr(nullptr);
    newNode->SetRightSiblingPtr(nullptr);
    newNode->SetMark(false);*/
    
    if (_min) {
        if (_min->GetRightSiblingPtr().get() == _min.get()) {
            _min->SetRightSiblingPtr(newNode);
            _min->SetLeftSiblingPtr(newNode);
            newNode->SetRightSiblingPtr(_min);
            newNode->SetLeftSiblingPtr(_min);
        } else {
            newNode->SetRightSiblingPtr(_min->GetRightSiblingPtr());
            newNode->SetLeftSiblingPtr(_min);
            newNode->GetRightSiblingPtr()->SetLeftSiblingPtr(newNode);
            _min->SetRightSiblingPtr(newNode);
        }
    } else {
        _min = newNode;
        newNode->SetRightSiblingPtr(newNode);
        newNode->SetLeftSiblingPtr(newNode);
    }

    if (comp(newNode->GetPriority(), _min->GetPriority())) {
        //_min.reset();
        _min = newNode;
    }
    ++size;
    pointersToNodes.emplace(count, newNode);
    newNode->SetPosition(count);
    ++count;

    return count - 1;
}

template <class TValue, class Compare>
void FibonacciHeap<TValue, Compare>::Pop() {
		pointersToNodes.at(_min->GetPosition()).reset();
		pointersToNodes.erase(_min->GetPosition());
		if (_min) {
			if (_min->GetChildPtr()) {
				std::vector<std::shared_ptr<node>> rootChildren;
				std::shared_ptr<node> start = _min->GetChildPtr();
				rootChildren.push_back(start);
				
                for (std::shared_ptr<node> ptr = start->GetRightSiblingPtr(); ptr.get() != start.get();) {
					rootChildren.push_back(ptr);
					std::shared_ptr<node> temp = ptr->GetRightSiblingPtr();
					ptr.reset();
					ptr = temp;
				}
				
                for (int i = 0; i < rootChildren.size(); ++i) {
					rootChildren[i]->SetParentPtr(nullptr);
					if (_min->GetRightSiblingPtr().get() == _min.get()) {
						_min->SetRightSiblingPtr(rootChildren[i]);
						_min->SetLeftSiblingPtr(rootChildren[i]);
						rootChildren[i]->SetRightSiblingPtr(_min);
						rootChildren[i]->SetLeftSiblingPtr(_min);
					} else {
						rootChildren[i]->SetRightSiblingPtr(_min->GetRightSiblingPtr());
						rootChildren[i]->SetLeftSiblingPtr(_min);
						rootChildren[i]->GetRightSiblingPtr()->SetLeftSiblingPtr(rootChildren[i]);
						_min->SetRightSiblingPtr(rootChildren[i]);
					}
				}
			}
			_min->GetRightSiblingPtr()->SetLeftSiblingPtr(_min->GetLeftSiblingPtr());
			_min->GetLeftSiblingPtr()->SetRightSiblingPtr(_min->GetRightSiblingPtr());
			if (_min.get() == _min->GetRightSiblingPtr().get()) {
				_min->clear();
				_min.reset();
			}
			else {
				std::shared_ptr<node> right = _min->GetRightSiblingPtr();
				_min->clear();
				_min.reset();
				_min = right;
				Consolidate();
			}
			--size;
		} else 
            throw PriorityQueueException("Heap is empty!"); 
	}

template <class TValue, class Compare>
std::shared_ptr<typename FibonacciHeap<TValue, Compare>::BasicFibonacciHeapNode> FibonacciHeap<TValue, Compare>::FindNode(const node& _node, std::shared_ptr<node> ptr) {
    if (*ptr == _node)
        return ptr;
    std::shared_ptr<node> result;
    if (ptr->GetChildPtr()) {
        result = FindNode(_node, ptr->GetChildPtr());
        if (result)
            return result;
        for (std::shared_ptr<node> start = ptr->GetChildPtr()->GetRightSiblingPtr(); start.get() != ptr->GetChildPtr().get();) {
            result = FindNode(_node, start);
            if (result)
                return result;
            else {
                std::shared_ptr<node> temp = start->GetRightSiblingPtr();
                //start.reset();
                start = temp;
            }
        }
    }
    if (ptr->GetRightSiblingPtr().get() != ptr.get()) {
        for (std::shared_ptr<node> start = ptr->GetRightSiblingPtr(); start.get() != ptr.get();) {
            result = FindNode(_node, start);
            if (result)
                return result;
            else {
                std::shared_ptr<node> temp = start->GetRightSiblingPtr();
                //start.reset();
                start = temp;
            }
        }
    }
    return result;
}

template <class TValue, class Compare>
void FibonacciHeap<TValue, Compare>::Cut(std::shared_ptr<node>& x, std::shared_ptr<node>& y) {
    if (x.get() == x->GetRightSiblingPtr().get()) {
        y->SetChildPtr(nullptr);
    } else {
        if (y->GetChildPtr().get() == x.get())
            y->SetChildPtr(x->GetRightSiblingPtr());
        x->GetRightSiblingPtr()->SetLeftSiblingPtr(x->GetLeftSiblingPtr());
        x->GetLeftSiblingPtr()->SetRightSiblingPtr(x->GetRightSiblingPtr());
    }
    if (_min->GetRightSiblingPtr().get() == _min.get()) {
        _min->SetRightSiblingPtr(x);
        _min->SetLeftSiblingPtr(x);
        x->SetRightSiblingPtr(_min);
        x->SetLeftSiblingPtr(_min);
    } else {
        x->SetRightSiblingPtr(_min->GetRightSiblingPtr());
        x->SetLeftSiblingPtr(_min);
        x->GetRightSiblingPtr()->SetLeftSiblingPtr(x);
        _min->SetRightSiblingPtr(x);
    }
    x->SetParentPtr(nullptr);
    x->SetMark(false);
}

template <class TValue, class Compare>
void FibonacciHeap<TValue, Compare>::FibHeapLink(std::shared_ptr<node> y, std::shared_ptr<node> x) {
    if (y->GetRightSiblingPtr().get() == y.get()) {
        _min.reset();
    } else {
        y->GetRightSiblingPtr()->SetLeftSiblingPtr(y->GetLeftSiblingPtr());
        y->GetLeftSiblingPtr()->SetRightSiblingPtr(y->GetRightSiblingPtr());
    }
    
    y->SetRightSiblingPtr(nullptr);
    y->SetLeftSiblingPtr(nullptr);
    y->SetParentPtr(x);
    
    if (x->GetChildPtr()) {
        if (x->GetChildPtr()->GetRightSiblingPtr().get() == x->GetChildPtr().get()) {
            x->GetChildPtr()->SetRightSiblingPtr(y);
            x->GetChildPtr()->SetLeftSiblingPtr(y);
            y->SetRightSiblingPtr(x->GetChildPtr());
            y->SetLeftSiblingPtr(x->GetChildPtr());
        } else {
            y->SetRightSiblingPtr(x->GetChildPtr()->GetRightSiblingPtr());
            y->SetLeftSiblingPtr(x->GetChildPtr());
            x->GetChildPtr()->GetRightSiblingPtr()->SetLeftSiblingPtr(y);
            x->GetChildPtr()->SetRightSiblingPtr(y);
        }
    } else {
        x->SetChildPtr(y);
        y->SetLeftSiblingPtr(y);
        y->SetRightSiblingPtr(y);
    }
    
    x->SetDegree(x->GetDegree() + 1);
    y->SetMark(false);
}

template <class TValue, class Compare>
void FibonacciHeap<TValue, Compare>::Consolidate() {
    std::vector<std::shared_ptr<node>> array (size);
    std::vector<std::shared_ptr<node>> rootSibling;
    std::shared_ptr<node> start = _min;
    rootSibling.push_back(start);
    for (std::shared_ptr<node> ptr = start->GetRightSiblingPtr(); ptr.get() != start.get();) {
        rootSibling.push_back(ptr);
        std::shared_ptr<node> temp = ptr->GetRightSiblingPtr();
        ptr.reset();
        ptr = temp;
    }
    
    for (int i = 0; i < rootSibling.size(); ++i) {
        std::shared_ptr<node> x = rootSibling[i];
        int d = x->GetDegree();
        while (array[d]) {
            std::shared_ptr<node> y = array[d];
            
            if (!comp(x->GetPriority(), y->GetPriority())) {
                int parent = y->GetPosition();
                int i = x->GetPosition();
                std::shared_ptr<node> temp = pointersToNodes.at(parent);
                pointersToNodes.at(i)->SetPosition(parent);
                pointersToNodes.at(parent)->SetPosition(i);
                pointersToNodes.at(parent).reset();
                pointersToNodes.at(parent) = pointersToNodes.at(i);
                pointersToNodes.at(i).reset();
                pointersToNodes.at(i) = temp;
                y->swap(x);
            }
            FibHeapLink(y, x);
            array[d].reset();
            ++d;
        } 
        array[d].reset();
        array[d] = x;
    }
    _min.reset();
    for (int i = 0; i < size; ++i) {
        if (array[i]) {
            array[i]->SetRightSiblingPtr(array[i]);
            array[i]->SetLeftSiblingPtr(array[i]);
            
            if (_min) {
                if (_min->GetRightSiblingPtr().get() == _min.get()) {
                    _min->SetRightSiblingPtr(array[i]);
                    _min->SetLeftSiblingPtr(array[i]);
                    array[i]->SetRightSiblingPtr(_min);
                    array[i]->SetLeftSiblingPtr(_min);
                } else {
                    array[i]->SetRightSiblingPtr(_min->GetRightSiblingPtr());
                    array[i]->SetLeftSiblingPtr(_min);
                    array[i]->GetRightSiblingPtr()->SetLeftSiblingPtr(array[i]);
                    _min->SetRightSiblingPtr(array[i]);
                }
            } else {
                _min = array[i];
            }
            
            if (!_min || comp(array[i]->GetPriority(), _min->GetPriority())) {
                _min.reset();
                _min = array[i];
            }

        }
    }
}

