#include <iostream>
using namespace std;

template <typename T>

struct TNode {
	TNode* Prev;
	TNode* Next;
	T Value;
};

template <typename T>
class TIterotor {
	TNode <T>* Node;
	T& operator*(){
		return Node-> Value;
	};
	TIterator& operator++() {
		Node=Node->Next;
		return *this;
	};
	TIterator operator ++(int){
		TIterator it;
		it.Node=Node;
		Node=Node->Next;
		return it;
	};
	bool operator==(cons TIterator &other){
		return Node==other.Node;
	};

	bool operator !=(cons TIterator &other){
		return !(*this ==other);
	};

};


class TList {
public: 
	typedef TIterator iteratr;
};