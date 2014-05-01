#include <iostream>
#include <memory>
#include <climits>
#include <cmath>
#include <vector>
#include <unordered_map>

template <class node> 
class IPriorityQueue {
public:
	virtual int Add(const node&) = 0;
	virtual node& GetMin() = 0;
	virtual node ExtractMin() = 0;
	virtual void UpdateKey(int, long long) = 0;
};

class HeapException: public std::exception {
	const char* message;
public:
	HeapException(const char* _message) {
		message = _message;
	}
	const char* what() {
		return message;
	}
};

class BasicHeapNode {
	long long priority;
public:
	unsigned int position;
	long long GetPriority() const {
		return priority;
	}	
	void SetPriority(long long _priority) {
		priority = _priority;
	}
};

template <typename ptr>
class BasicBinominalHeapNode: public BasicHeapNode {
	std::shared_ptr<ptr> rightSibling;
	std::shared_ptr<ptr> parent;
	std::shared_ptr<ptr> child;
	int degree;
public:
	void SetParentPtr(std::shared_ptr<ptr> _parent) {
		parent.reset();
		parent = std::move(_parent);
	}
	void SetChildPtr(std::shared_ptr<ptr> _child) {
		child.reset();
		child = _child;
	}
	void SetRightSiblingPtr(std::shared_ptr<ptr> _rightSibling) {
		rightSibling.reset();
		rightSibling = _rightSibling;
	}
	void SetDegree(int _degree) {
		degree = _degree;
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
	int GetDegree() const {
		return degree;
	}
	virtual void swapContent (ptr& another) = 0;
	virtual void clear() {
		child.reset();
		parent.reset();
		rightSibling.reset();
	}
};

template <typename ptr> 
class BasicFibonacciHeapNode: public BasicBinominalHeapNode<ptr> {
    typedef BasicBinominalHeapNode<ptr> Parent;
	std::shared_ptr<ptr> leftSibling;
	bool mark;
public:
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
		if (Parent::GetRightSiblingPtr().get() == this)
			Parent::SetRightSiblingPtr(nullptr);
	}
	std::shared_ptr<ptr> GetLeftSiblingPtr() const {
		return leftSibling;
	}
	bool GetMark() const {
		return mark;
	}
	virtual void clear() {
		leftSibling.reset();
		Parent::SetChildPtr(nullptr);
		Parent::SetParentPtr(nullptr);
		Parent::SetRightSiblingPtr(nullptr);
	}
	void swap(std::shared_ptr<ptr> another) {
		std::shared_ptr<ptr> tempParent = Parent::GetParentPtr();
		std::shared_ptr<ptr> tempChild = Parent::GetChildPtr();
		Parent::SetParentPtr(another->GetParentPtr());
		Parent::SetChildPtr(another->GetChildPtr());
		another->SetParentPtr(tempParent);
		another->SetChildPtr(tempChild);
		this->swapContent(*another);
	}
};


template <class node>
class BinaryHeap: public IPriorityQueue<node> {
	std::vector<node> array;
	std::unordered_map<unsigned int,unsigned int> pointersArray;
	int count;
public:
	BinaryHeap() {
		node temp;
		array.push_back(temp);
		count = 0;
	}
	~BinaryHeap() {
		node check;
		BasicHeapNode* _check = dynamic_cast<BasicHeapNode*>(&check);
	}
	node& GetMin() {
		if (array.empty()) {
			throw HeapException("GetMin: binominal heap is empty!");
		}
		return array[1];
	}
	node ExtractMin() {
		if (array.empty()) {
			throw HeapException("GetMin: binominal heap is empty!");
		}
		node _min = array[1];
		int deletedPosition = array[1].position;
		array[1] = array.back();
		pointersArray[array.back().position] = 1;
		array.pop_back();
		Heapify(1);
		pointersArray.erase(deletedPosition);
		return _min;
	}

	void UpdateKey(int positionOfNode, long long newKey) {
		if (array[pointersArray.at(positionOfNode)].GetPriority() < newKey)
			throw HeapException("UpdateKey: trying to increase key!");
		array[pointersArray.at(positionOfNode)].SetPriority(newKey);
		UpdateKey(pointersArray.at(positionOfNode));
	}

	int Add(const node& _node) {
		array.push_back(_node);
		array.back().position = count;
		pointersArray.emplace(count, array.size() - 1);
		array.back().SetPriority(_node.GetPriority());
		array.back().position = count;
		UpdateKey(array.size() - 1);
		++count;
		return count - 1;
	}

private:
	void UpdateKey(int i) {
		const int initialPosition = i;
		while (i > 1 && array[Parent(i)].GetPriority() > array[i].GetPriority()){
			std::swap(pointersArray.at(array[i].position), pointersArray.at(array[Parent(i)].position));
			std::swap(array[i], array[Parent(i)]);
			i = Parent(i);
		}
	}

	unsigned int Parent(int i){
		return i/2;
	}
	unsigned int LeftChild(int i){
		return 2*i;
	}
	unsigned int RightChild(int i){
		return 2*i+1;
	}

	void Heapify(unsigned int i){
		unsigned int l = LeftChild(i);
		unsigned int r = RightChild(i);
		unsigned int largest;
		if ( l < array.size() && array[l].GetPriority() < array[i].GetPriority())
			largest = l;
		else
			largest = i;
		if (r < array.size() && array[r].GetPriority() < array[largest].GetPriority())
			largest = r;
		if (largest != i) {
			std::swap(pointersArray.at(array[i].position), pointersArray.at(array[largest].position));
			std::swap(array[i], array[largest]);
			Heapify(largest);
		}
	}
};




template <class node>
class BinominalHeap:public IPriorityQueue<node> {
	std::shared_ptr<node> head;
	std::unordered_map<int, std::shared_ptr<node>> pointersToNodes;
	int count;
	void PrintTree(std::shared_ptr<node>& _ptr) {
		std::cout <<"Node id =" << _ptr->GetID();
		std::cout <<"\tpriority =" << _ptr->GetPriority() <<"\t parent = ";
		if (_ptr->GetParentPtr())
			std::cout << _ptr->GetParentPtr()->GetPriority() <<"\t child = ";
		else 
			std::cout <<"null\t child = ";
		if (_ptr->GetChildPtr())
			std::cout << _ptr->GetChildPtr()->GetPriority() <<"\t sibling = ";
		else 
			std::cout <<"null\t sibling = ";
		if (_ptr->GetRightSiblingPtr())
			std::cout << _ptr->GetRightSiblingPtr()->GetPriority() <<std::endl;
		else 
			std::cout <<"null" << std::endl;		 
		if (_ptr->GetRightSiblingPtr())
			PrintTree(_ptr->GetRightSiblingPtr());
		if (_ptr->GetChildPtr())
			PrintTree(_ptr->GetChildPtr());
	}

public:
	BinominalHeap() {
		count = 0;
	}
	~BinominalHeap() {
		node check;
		BasicBinominalHeapNode<node>* _check = dynamic_cast<BasicBinominalHeapNode<node>*>(&check);
	}
	node& GetMin() {
		if (!head) {
			throw HeapException("GetMin: binominal heap is empty!");
		}
		std::shared_ptr<node> y;
		std::shared_ptr<node> x = head;
		long long _min = LLONG_MAX; //std::numeric_limits<long long>::max();
		while (x) {
			if (x->GetPriority() < _min) {
				_min = x->GetPriority();
				y.reset();
				y = x;
			}
			std::shared_ptr<node> temp = x->GetRightSiblingPtr();
			x.reset();
			x = temp;
		}
		return *y;
	}

	int Add(const node& _node) {
		std::shared_ptr<node> newNode (new node);
		*newNode = _node; // here was pointing child .. to nullptr
		newNode->SetDegree(0);
		newNode->position = count;
		pointersToNodes.emplace(count, newNode);
		BinominalHeapUnion(newNode, head);
		++count;
		return count - 1;
	}

	node ExtractMin() {
		std::shared_ptr<node> y;
		std::shared_ptr<node> x = head;
		std::shared_ptr<node> prev;
		std::shared_ptr<node> previous;
		long long _min = LLONG_MAX;//std::numeric_limits<long long>::max();
		while (x) {
			if (x->GetPriority() < _min) {
				_min = x->GetPriority();
				y.reset();
				previous.reset();
				previous = std::move(prev);
				y = x;
			}
			std::shared_ptr<node> temp = x->GetRightSiblingPtr();
			prev = std::move(x);
			x = temp;
		}
		if (!previous) {
			head.reset();
			head = y->GetRightSiblingPtr();
		} else {
			previous->SetRightSiblingPtr(y->GetRightSiblingPtr());
		}
		std::shared_ptr<node> children;
		if (y->GetChildPtr()) {
			if (y->GetChildPtr()->GetRightSiblingPtr()) {
				ReverseChildren(y->GetChildPtr(), y->GetChildPtr()->GetRightSiblingPtr(), children);
			} else {
				children = y->GetChildPtr();
				y->GetChildPtr()->SetParentPtr(nullptr);
			}
			BinominalHeapUnion(head, children);
		}
		pointersToNodes.erase(y->position);
		return *y;
	}

	void UpdateKey(int i, long long newKey) {
		if (pointersToNodes.at(i)->GetPriority() < newKey)
			throw HeapException("UpdateKey: new priority is higher than old one!");
		pointersToNodes.at(i)->SetPriority(newKey);
		if (pointersToNodes.at(i)->GetParentPtr()) {
			int parent = pointersToNodes.at(i)->GetParentPtr()->position;
			while (parent >= 0 && pointersToNodes.at(i)->GetPriority() < pointersToNodes.at(parent)->GetPriority()) {
				int tempParent = -1;
				if (pointersToNodes.at(parent)->GetParentPtr())
					tempParent = pointersToNodes.at(parent)->GetParentPtr()->position;
				std::shared_ptr<node> temp = pointersToNodes.at(parent);
				pointersToNodes.at(i)->swapContent(*pointersToNodes.at(parent));
				pointersToNodes.at(i)->position = parent;
				pointersToNodes.at(parent)->position = i;
				pointersToNodes.at(parent).reset();
				pointersToNodes.at(parent) = pointersToNodes.at(i);
				pointersToNodes.at(i).reset();
				pointersToNodes.at(i) = temp;
				parent = tempParent;
			}
		}
	} 

private:

	std::shared_ptr<node> FindNode(const node& _node, std::shared_ptr<node>& ptr) {
		if (*ptr == _node) {
			return ptr;
		}
		std::shared_ptr<node> temp;
		if (ptr->GetChildPtr())
			temp = FindNode(_node, ptr->GetChildPtr());
		if (temp)
			return temp;
		if (ptr->GetRightSiblingPtr())
			temp = FindNode(_node, ptr->GetRightSiblingPtr());
		return temp;
	}

	void ReverseChildren (std::shared_ptr<node> first, std::shared_ptr<node> second, std::shared_ptr<node>& children) {
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

	void BinominalLink(std::shared_ptr<node>& y, std::shared_ptr<node>& z) {
		y->SetParentPtr(z);
		y->SetRightSiblingPtr(z->GetChildPtr());
		z->SetChildPtr(y);
		z->SetDegree(z->GetDegree() + 1);
	}
	std::shared_ptr<node> BinominalHeapMerge(std::shared_ptr<node> first, std::shared_ptr<node> second) {
		std::shared_ptr<node> finalResult;
		std::shared_ptr<node> temp;
		if (first)
			finalResult = first;
		else 
			finalResult = second;
		for (int i = 0; first && second; ++i) {
			if (first->GetDegree() <= second->GetDegree()) {
				if (i == 0) {
					finalResult.reset();
					finalResult = first;
				}
				temp.reset();
				temp = first;
				std::shared_ptr<node> _temp = first->GetRightSiblingPtr();
				first.reset();
				first = _temp;
			} else {
				if (i == 0) {
					finalResult.reset();
					finalResult = second;
				}
				if (temp)
					temp->SetRightSiblingPtr(second);
				std::shared_ptr<node> _temp = second->GetRightSiblingPtr();
				second->SetRightSiblingPtr(first);
				temp.reset();
				temp = second;	
				second.reset();
				second = _temp;
			}
		}
		if (!first && temp)
			temp->SetRightSiblingPtr(second);
		return finalResult;
	}

	void BinominalHeapUnion(std::shared_ptr<node> first, std::shared_ptr<node> second) {
		head.reset();
		head = std::move(BinominalHeapMerge(first, second));
		if (!head)
			return;
		std::shared_ptr<node> prev_x;
		std::shared_ptr<node> x = head;
		std::shared_ptr<node> next_x = x->GetRightSiblingPtr();
		while (next_x) {
			if (x->GetDegree() != next_x->GetDegree() 
				|| (next_x->GetRightSiblingPtr() && next_x->GetRightSiblingPtr()->GetDegree() == x->GetDegree())) {
					prev_x.reset();
					prev_x = std::move(x);
					x.reset();
					x = std::move(next_x);
			} else {
				if (x->GetPriority() <= next_x->GetPriority()) {
					x->SetRightSiblingPtr(next_x->GetRightSiblingPtr());
					BinominalLink(next_x, x);
				} else {
					if (!prev_x) {
						head.reset();
						head = next_x;
					} else {
						prev_x->SetRightSiblingPtr(next_x);
					}
					BinominalLink(x, next_x);
					x.reset();
					x = std::move(next_x);
				}
			}
			next_x.reset();
			next_x = x->GetRightSiblingPtr();
		}
	}

};

template <class node>
class FibonacciHeap:public IPriorityQueue<node> {
	std::shared_ptr<node> _min;
	std::unordered_map<int, std::shared_ptr<node>> pointersToNodes;
	int count;
	int n;
	void PrintTreeSibling(std::shared_ptr<node> _ptr) {
		if (!_ptr)
			return;
		std::cout <<"Node id =" << _ptr->GetID() <<"\t priority = "<<_ptr->GetPriority() <<"\t parent = ";
		if (_ptr->GetParentPtr())
			std::cout << _ptr->GetParentPtr()->GetID() <<"\t child = ";
		else 
			std::cout <<"null\t child = ";
		if (_ptr->GetChildPtr())
			std::cout << _ptr->GetChildPtr()->GetID() <<"\t rightSibling = ";
		else 
			std::cout <<"null\t rightSibling = ";
		if (_ptr->GetRightSiblingPtr())
			std::cout << _ptr->GetRightSiblingPtr()->GetID() <<"\t leftSibling = ";
		else 
			std::cout <<"null\t leftSibling = ";
		if (_ptr->GetLeftSiblingPtr())
			std::cout << _ptr->GetLeftSiblingPtr()->GetID() <<std::endl;
		else 
			std::cout <<"null" << std::endl;
		if (_ptr->GetChildPtr())
			PrintTreeHelp(_ptr->GetChildPtr());
	}

	void PrintTreeHelp (std::shared_ptr<node> _ptr) {
		if (!_ptr)
			return;
		std::cout <<"Node id =" << _ptr->GetID() <<"\t priority = "<<_ptr->GetPriority() <<"\t parent = ";
		if (_ptr->GetParentPtr())
			std::cout << _ptr->GetParentPtr()->GetID() <<"\t child = ";
		else 
			std::cout <<"null\t child = ";
		if (_ptr->GetChildPtr())
			std::cout << _ptr->GetChildPtr()->GetID() <<"\t rightSibling = ";
		else 
			std::cout <<"null\t rightSibling = ";
		if (_ptr->GetRightSiblingPtr())
			std::cout << _ptr->GetRightSiblingPtr()->GetID() <<"\t leftSibling = ";
		else 
			std::cout <<"null\t leftSibling = ";
		if (_ptr->GetLeftSiblingPtr())
			std::cout << _ptr->GetLeftSiblingPtr()->GetID() <<std::endl;
		else 
			std::cout <<"null" << std::endl;
		if (_ptr.get() != _ptr->GetRightSiblingPtr().get()) {
			for (std::shared_ptr<node> temp = _ptr->GetRightSiblingPtr(); temp.get() != _ptr.get();) {
				PrintTreeSibling(temp);
				std::shared_ptr<node> _temp = temp->GetRightSiblingPtr();
				temp.reset();
				temp = _temp;
			}
		}
		if (_ptr->GetChildPtr())
			PrintTreeHelp(_ptr->GetChildPtr());
	}

	void PrintTree(std::shared_ptr<node> _ptr) {
		std::cout <<"Start----------------------------------------------------------------------\n";
		PrintTreeHelp(_ptr);
		std::cout <<"End----------------------------------------------------------------------\n";
	}

public:
	FibonacciHeap() {
		n = 0;
		count = 0;
	}
	~FibonacciHeap() {
		node check;
		BasicFibonacciHeapNode<node>* _check = dynamic_cast<BasicFibonacciHeapNode<node>*>(&check);
	}

	int Add(const node& _node) {
		std::shared_ptr<node> newNode (new node);
		*newNode = _node;
		newNode->SetDegree(0);
		newNode->SetParentPtr(nullptr);
		newNode->SetChildPtr(nullptr);
		newNode->SetLeftSiblingPtr(nullptr);
		newNode->SetRightSiblingPtr(nullptr);
		newNode->SetMark(false);
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
			_min.reset();
			_min = newNode;
		}
		++n;
		pointersToNodes.emplace(count, newNode);
		newNode->position = count;
		++count;
		return count - 1;
	}

	node& GetMin() {
		return *_min;
	}

	node ExtractMin() {
		node z = *_min;
		//PrintTree(_min);
		pointersToNodes.at(_min->position).reset();
		pointersToNodes.erase(_min->position);
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
			--n;
		}
		return z;
	}

	void UpdateKey(int i, long long newKey) {
		std::shared_ptr<node> x = pointersToNodes.at(i);
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
			_min.reset();
			_min = x;
		}
	}

private:
	std::shared_ptr<node> FindNode(const node& _node, std::shared_ptr<node> ptr) {
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
					start.reset();
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
					start.reset();
					start = temp;
				}
			}
		}
		return result;
	}

	void Cut(std::shared_ptr<node>& x, std::shared_ptr<node>& y) {
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

	void FibHeapLink(std::shared_ptr<node> y, std::shared_ptr<node> x) {
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

	void Consolidate() {
		std::vector<std::shared_ptr<node>> array (n);
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
					int parent = y->position;
					int i = x->position;
					std::shared_ptr<node> temp = pointersToNodes.at(parent);
					pointersToNodes.at(i)->position = parent;
					pointersToNodes.at(parent)->position = i;
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
		for (int i = 0; i < n; ++i) {
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
};


