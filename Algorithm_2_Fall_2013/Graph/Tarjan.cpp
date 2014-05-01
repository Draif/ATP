#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

struct node {
	int i;
	bool operator<(const node& a){
		return i < a.i;
	}
	bool operator== ( const node& another){
		return i == another.i;
	}
};

template <class TNode>
class ConstVertexIterator{
	typename map< TNode, list <TNode&> >:: const_iterator tp;
public:
	ConstVertexIterator (typename map <TNode, list <TNode&>>:: const_iterator& a){
		tp = a;
	}
	~ConstVertexIterator();

	const TNode& operator*() const{
		return tp->first;
	}

	ConstVertexIterator<TNode>& operator++ (){
		return ++tp;
	}

	bool operator==( ConstVertexIterator<TNode>& anoter){
		return (tp == *anoter)? true: false;
	}

};

template <class TNode>
class Graph{
	map < TNode, list <TNode*> >adj;
public:
	typedef typename list<TNode*>:: const_iterator ConstEdgeIterator;
	Graph(){
	}

	Graph(vector <TNode>& v){
		list <TNode*> l;
		vector <TNode> :: iterator ptr = v.begin();
		for ( int i = 0;
			ptr !=  v.end(); ++ptr, ++i)
			adj [*ptr] = l;
	}
	~Graph(){ }

	void AddEdge( const TNode& a, const TNode& b){
		map:: iterator ptr = adj.find(b);
		adj[ a ].push_back(ptr.first);
	}

	bool HasEdge (const TNode& a, const TNode& b) const {
		ConstEdgeIterator  ptr = adj[a].begin();
		for ( ;	ptr !=  adj[a].end(); ++ptr)
			if ( *ptr == b)
				break;
		return ( ptr == adj[a].end())? false: true;
	}

	//shared_ptr<Edge<TNode>> GetEdge (const TNode& a, const TNode& b) const; 

	ConstVertexIterator<TNode> begin() const{
	}
	ConstVertexIterator<TNode> end() const{
	}
	
	ConstEdgeIterator GetNeighboursBegin( const TNode& a) const{
		return map[a].cbegin();
	}

	ConstEdgeIterator GetNeighboursEnd (const TNode& b) const{
		return map[a].cend();
	}


};



int main() {
	Graph<node> a;
	list <node&> b;
	return 0;
}