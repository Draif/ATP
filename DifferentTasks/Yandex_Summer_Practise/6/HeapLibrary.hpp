#ifndef HEAP_LIBRARY
#define HEAP_LIBRARY
#include <iostream>
#include <memory>
#include <climits>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
/*
Interface for PriorityQueueu
*/
template <class TValue> 
class IPriorityQueue {
public:
    virtual bool Empty() const = 0;

    virtual size_t Size() const = 0;
    
    virtual long long Push (const TValue&, long long priority) = 0;
	
    virtual const TValue& Top() const = 0;

    virtual void Pop() = 0;
	
    virtual void UpdateKey(long long, long long) = 0;
};

class HeapException: public std::exception {
	std::string message;
public:
	HeapException(const char* _message) : message(_message) {}
	HeapException(const HeapException& another) {
        message = another.message;
    }
    const char* what() const noexcept {
		return message.c_str();
	}
};

//Basic Node, which is used in PriorityQueue. 
//Store just priority and value, contained in PQ.
template <class TValue>
class BasicHeapNode {
public:
    BasicHeapNode(const TValue& _val, long long pr) : val(_val), priority(pr) {}
    BasicHeapNode(const BasicHeapNode& another) {
        *this = another;
    }
    BasicHeapNode& operator=(const BasicHeapNode& another) {
        priority = another.priority;
        val = another.val;
        position = another.position;
        return *this;
    }
    
    void SetPosition(long long pos) {
        position = pos;
    }
    long long GetPosition() const {
        return position;
    }

	long long GetPriority() const {
		return priority;
	}	
	void SetPriority(long long _priority) {
		priority = _priority;
	}
    
    const TValue& GetValue() const {
        return val;
    }
    
    void swapContent(BasicHeapNode& another) {
        std::swap(val, another.val);
    }
    
private:
	unsigned int position;
    long long priority;
	TValue val;
};

class Less {
public:
    bool operator()(long long prior_a, long long prior_b) const {
        return prior_a <= prior_b;
    }
};

/*      Usual Binary Heap with priorities.
        Additional explanation you can watch at http://en.wikipedia.org/wiki/Binary_heap.
        As template parametrs gets type of class stored in heap and one optional parametrs,
        which allow compare priorities. By Default - it is minimizing heap.
*/
template <class TValue, class Compare = Less>
class BinaryHeap: public IPriorityQueue<TValue> {
public:
	BinaryHeap() : count(0), size(0) {}
    
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
		if (size == 0) {
			throw HeapException("GetMin: binominal heap is empty!");
		}
		return array.front().GetValue();
	}

    //extract top element
	void Pop() {
		if (size == 0) {
			throw HeapException("GetMin: binominal heap is empty!");
		}
		size_t deletedPosition = array.front().GetPosition();
		array.front() = array.back();
		pointersArray[array.back().GetPosition()] = 0;
		array.pop_back();
		Heapify(0);
		pointersArray.erase(deletedPosition);
        --size;
    }

    //allow update key for element already in heap.
	void UpdateKey(long long positionOfNode, long long newKey) {
		if (!comp(newKey, array[pointersArray.at(positionOfNode)].GetPriority()))
			throw HeapException("UpdateKey: compare(newKey, oldKey) return false");
		array[pointersArray.at(positionOfNode)].SetPriority(newKey);
		UpdateKeyHelp(pointersArray.at(positionOfNode));
	}

    //Push elemet in heap. Return unique indificator of new element, which can be used in UpdateKey.
	long long Push(const TValue& val, long long priority) {
		array.push_back(BasicHeapNode<TValue>(val, priority));
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
		const int initialPosition = i;
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

    //Shift down. Used in Pop
	void Heapify(unsigned int i){
		unsigned int l = LeftChild(i);
		unsigned int r = RightChild(i);
		unsigned int largest;
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

    Compare comp;
    size_t size;
	std::vector<BasicHeapNode<TValue>> array;
	std::unordered_map<long long, size_t> pointersArray; //map unique index of element(returned by Push) and its place in array. Used in UpdateKey 
	long long  count;
};

/*      Usual Binominal Heap.
        More information at http://en.wikipedia.org/wiki/Binomial_heap or
        in T.H. Cormen, ... "Introduction to algorithms".
        By default - minimizing heap. Can be maximizing.
*/
template <class TValue, class Compare = Less>
class BinominalHeap : public IPriorityQueue<TValue> {
    class BasicBinominalHeapNode: public BasicHeapNode<TValue> {
        typedef BasicBinominalHeapNode ptr;
    public:
        BasicBinominalHeapNode(const TValue& _val, long long prior) : BasicHeapNode<TValue>(_val, prior), degree(0) {}
        
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
            child.reset();
            parent.reset();
            rightSibling.reset();
        }
    
    private:
        std::shared_ptr<BasicBinominalHeapNode> rightSibling;
        std::shared_ptr<BasicBinominalHeapNode> parent;
        std::shared_ptr<BasicBinominalHeapNode> child;
        int degree; 
    };

    typedef BinominalHeap<TValue, Compare>::BasicBinominalHeapNode node;
public:
	BinominalHeap() : count(0), size(0) {}
    
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
		if (Empty()) {
			throw HeapException("GetMin: binominal heap is empty!");
		}
		std::shared_ptr<node> y;
		std::shared_ptr<node> x = head;
		long long _min = LLONG_MAX; 
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
	long long Push(const TValue& _node, long long priority) {
		std::shared_ptr<node> newNode (new node(_node, priority));
		newNode->SetDegree(0);
		newNode->SetPosition(count);
		pointersToNodes.emplace(count, newNode);
		BinominalHeapUnion(newNode, head);
		++count;
        ++size;
		return count - 1;
	}
    //Pop element from heap
	void Pop ();
    //Change key for element in heap. Uses unique id, got from Push
	void UpdateKey(long long, long long );

private:
    
    

    //Helper function, used in HeapUnion
	void BinominalLink(std::shared_ptr<node>& y, std::shared_ptr<node>& z) {
		y->SetParentPtr(z);
		y->SetRightSiblingPtr(z->GetChildPtr());
		z->SetChildPtr(y);
		z->SetDegree(z->GetDegree() + 1);
	}

    //helper function, used in HeapUnion
	void ReverseChildren (std::shared_ptr<node>, std::shared_ptr<node>, std::shared_ptr<node>&);
    
    //helper function, used in HeapUnion
    std::shared_ptr<node> BinominalHeapMerge(std::shared_ptr<node>, std::shared_ptr<node>); 
	
    //helper function, in Push, Pop 
    void BinominalHeapUnion(std::shared_ptr<node>, std::shared_ptr<node>);
    
    Compare comp;
	std::shared_ptr<node> head;
	std::unordered_map<long long, std::shared_ptr<node>> pointersToNodes;
	long long count;
    int size;
};

//Below are declaration of member-function, defined in class BinominalHeap.
template <class TValue, class Compare>
void BinominalHeap<TValue, Compare>::Pop() {
    std::shared_ptr<node> y;
    std::shared_ptr<node> x = head;
    std::shared_ptr<node> prev;
    std::shared_ptr<node> previous;
    std::shared_ptr<node> children;
    long long _min = LLONG_MAX;

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

template <class TValue, class Compare>
void BinominalHeap<TValue, Compare>::UpdateKey(long long nodeNumber, long long newKey) {
    if (!comp(newKey, pointersToNodes.at(nodeNumber)->GetPriority()))
        throw HeapException("UpdateKey: new priority is higher than old one!");
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

template <class TValue, class Compare>
void BinominalHeap<TValue, Compare>::ReverseChildren (std::shared_ptr<node> first, std::shared_ptr<node> second, std::shared_ptr<node>& children) {
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

template <class TValue, class Compare>
std::shared_ptr<typename BinominalHeap<TValue, Compare>::BasicBinominalHeapNode> BinominalHeap<TValue, Compare>::BinominalHeapMerge(std::shared_ptr<node> first, std::shared_ptr<node> second) {
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
template <class TValue, class Compare>
void BinominalHeap<TValue, Compare>::BinominalHeapUnion(std::shared_ptr<node> first, std::shared_ptr<node> second) {
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

template <class TValue, class Compare = Less>
class FibonacciHeap : public IPriorityQueue<TValue> {
    class BasicFibonacciHeapNode: public BasicHeapNode<TValue> {
        typedef FibonacciHeap<TValue, Compare>::BasicFibonacciHeapNode Parent;
        typedef FibonacciHeap<TValue, Compare>::BasicFibonacciHeapNode ptr;
    public:
        BasicFibonacciHeapNode(const TValue& _val, long long prior) : BasicHeapNode<TValue>(_val, prior), mark(false), degree(0) {} 
        
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
        
        const TValue& GetValue() const {
            return val;
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
        TValue val;
        std::shared_ptr<ptr> leftSibling;
        std::shared_ptr<ptr> rightSibling;
        std::shared_ptr<ptr> parent;
        std::shared_ptr<ptr> child;
        int degree; 
        bool mark;
    };

    typedef BasicFibonacciHeapNode node;
public:
	FibonacciHeap() : size(0), count(0) {}
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
			throw HeapException("UpdateKey: no such node!");
		if (x->GetPriority() < newKey)
			throw HeapException("UpdateKey: new priority is higher than old one!");
		x->SetPriority(newKey);
		std::shared_ptr<node> y = x->GetParentPtr();
		if (y && x->GetPriority() < y->GetPriority()) {
			Cut(x, y);
			CascadingCut(y);
		}
		if (x->GetPriority() < _min->GetPriority()) {
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

    if (newNode->GetPriority() < _min->GetPriority()) {
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
		//node z = *_min;
		//PrintTree(_min);
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
            throw HeapException("Heap is empty!"); 
		//return z;
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
            
            if (x->GetPriority() > y->GetPriority()) {
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
            
            if (!_min || array[i]->GetPriority() < _min->GetPriority()) {
                _min.reset();
                _min = array[i];
            }

        }
    }
}

#endif
