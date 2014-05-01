#include <vector>
#include <map>
#include <algorithm>

struct vertex {
	int distance;
	int index;
	
	vertex (){
		index = -1;
		distance = -1;
	}
	vertex ( int i ){
		index = i;
	}

	vertex ( int i, int d){
		index = i;
		distance = d;
	}

	bool operator< ( const vertex& a) const{
		return index < a.index;
	}

	bool operator== ( const vertex& a) const {
		return index == a.index;
	}
};

template <class TVertex>
struct Compare {
	bool operator() ( const TVertex& a, const TVertex& b) const {
		return a.index < b.index;
	}
};


template < class T, class compare = Compare <T> >
class DijkstraHeap {
	std::vector<T> heap;
	std::map<T, int, Compare<T>> placement;
	typedef typename std::vector<T>::iterator Iter;
	int count;

	void Swap (int a1, int b1) {
		placement [heap[a1]] = b1;
		placement [heap[b1]] = a1;
		std::swap (heap[a1], heap[b1]);	
	}


	void Heapify(int i){
		unsigned int l = 2*i;
		unsigned int r = 2*i +1;
		unsigned int largest;
		if (l < heap.size() && compare()(heap[l], heap[i]) )
			largest=l;
		else
			largest=i;
		if (r < heap.size() && compare()(heap[r], heap[largest]))
			largest=r;
		if( largest != i ){
			Swap(i, largest);
			Heapify(largest);
		}
	}

public:
	DijkstraHeap() {
		count = 0;
	}

	DijkstraHeap(std::vector<T>& v) {
		count = v.size() - 1;
		heap = v;
		Iter ptr = v.begin();
		for ( int i = 0; ptr != v.end(); ++ptr, ++i)
			placement[*ptr] = i;
	}

	void MakeHeap() {
		for (int i = heap.size(); i >= 0; --i)
			Heapify(i);
	}
	void PushHeap(T a) {
		//a.index = count++;
		heap.push_back(a);
		placement[ heap[heap.size() - 1] ] = heap.size() - 1;
		MakeHeap();
	}
	
	T PopHeap() {
		if ( heap.size() > 0){
			T temp = heap[0];
			placement.erase( temp );
			heap.erase ( heap.begin());
			for ( typename std :: map < T, int> :: iterator ptr = placement.begin(); ptr != placement.end(); ++ptr)
				--ptr->second;
			MakeHeap();
			return temp;
		}
		else 
			return T ();
	}

	bool Update(T& w) {
		if ( std:: find ( heap.begin(), heap.end(), w) == heap.end() )
			return false;
		heap [ placement [w] ] = w;
		MakeHeap();
		return true;
	}

	Iter begin() {
		return heap.begin();
	}
	Iter end() {
		return heap.end();
	}


	T top() {
		return heap[0];
	}

	bool Empty() {
		return (heap.size() == 0);
	}

	bool IsInHeap(const T& Node) {
		if (placement.find(Node) != placement.end())
			return true;
		return false;
	}

};
