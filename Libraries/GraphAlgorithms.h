#include <queue>
#include <limits>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include "DijekstraHeap.h"
#include "Graph.h"

template <class TNode>
void DFS_VISIT (Graph<TNode>& a, TNode& v, int* time){
	v.color = GREY;
	v.openTime = ++ *time;
	for ( EdgeIterator<TNode> ptr = a.GetNeighboursBegin(v);
		ptr != a.GetNeighboursEnd(v); ++ ptr)
		if ( (*ptr).to->color == WHITE)
			DFS_VISIT(a, *((*ptr).to), time);
	v.color = BLACK;
	v.closeTime = ++ *time;
}

template <class TNode, class TEdge>
void DFS( Graph<TNode>& a){
	a.ClearVertices();
	int time = 0;
	for ( VertexIterator<TNode, TEdge> ptr = a.begin();
		ptr != a.end(); ++ptr)
		if ( (*ptr).color == WHITE)
			DFS_VISIT(a, *ptr, &time);
}

template <class TNode>
void BFS (Graph <TNode>& a, TNode& v){
	std:: queue <TNode*> q;
	TNode* temp = &(*(a.Find(v))); 
	a.ClearVertices();
	temp->distance = 0;
	temp->color = GREY;
	q.push(temp);
	while ( ! q.empty()){
		temp = q.front();
		q.pop();
		for ( EdgeIterator<TNode> ptr = a.GetNeighboursBegin(*temp);
			ptr != a.GetNeighboursEnd(*temp); ++ ptr)
			if (  (*ptr).to->color == WHITE ){
				(*ptr).to->parent = temp;
				(*ptr).to->distance = temp->distance + 1;
				(*ptr).to->color = GREY;
				q.push((*ptr).to);
			}
		temp->color = BLACK;
	}
}

template <class TNode, class TEdge>
void TarjanStrongConnect ( Graph<TNode>& a, VertexIterator<TNode, TEdge> v, int& index, \ 
    std::vector <VertexIterator<TNode, TEdge>>& S, std::vector<std::vector <VertexIterator<TNode, TEdge>>>& vec) {
	v->Tarjan_index = index;
	v->Tarjan_low_index = index;
	++index;
	S.push_back(v);
	for ( auto ptr = a.GetNeighboursBegin(*v);
			ptr != a.GetNeighboursEnd(*v); ++ ptr){
				if ( (*ptr).to->Tarjan_index == -1) {
					TarjanStrongConnect(a, a.Find(*((*ptr).to)), index, S, vec);
					v->Tarjan_low_index = std::min (v->Tarjan_low_index, (*ptr).to->Tarjan_low_index);
				} else if (find( S.begin(), S.end(), a.Find(*((*ptr).to))) != S.end())
					v->Tarjan_low_index = std::min (v->Tarjan_low_index, (*ptr).to->Tarjan_index);
	}
	if ( v->Tarjan_low_index == v->Tarjan_index) {
		std::vector <VertexIterator<TNode, TEdge>> temp;
		do {
			temp.push_back (S[S.size() - 1]);
			temp.back()->numberOfSSC = vec.size();
			S.pop_back();
		} while (temp[temp.size() - 1] != v);
		vec.push_back(temp);
	}
}




template <class TNode, class TEdge>
std::vector<std::vector<VertexIterator<TNode, TEdge>>> Tarjan_SSC(Graph <TNode>& a) {
	a.ClearVertexes();
	std::vector<VertexIterator<TNode, TEdge>> S;
	int index = 0;
	std::vector <std::vector<VertexIterator<TNode, TEdge>>> vec; 
	for ( VertexIterator<TNode, TEdge> ptr = a.begin();
		ptr != a.end(); ++ptr)
		if ( (*ptr).Tarjan_index == -1)
			TarjanStrongConnect (a, ptr, index, S,  vec);
	return vec;
}


template <class TNode>
class PrimCompare {
public:
	bool operator() (const TNode& a, const TNode& b) const {
		return (a.distance) < (b.distance);
	}
};


template <class TNode>
std::vector<edge<TNode>> MST_PRIM (Graph<TNode>& a) {
	std::vector <edge<TNode>> result;
	std::map <TNode, edge<TNode>> edges;
	std::map <TNode, bool> isInHeap;
	DijkstraHeap<TNode, PrimCompare<TNode>> heap;
	auto ptr = a.begin();
	ptr->distance = 0;
	heap.PushHeap(*ptr);
	isInHeap[*ptr] = true;
	for (++ptr; ptr != a.end(); ++ptr) {
		ptr->distance = std::numeric_limits<int>::max();
		heap.PushHeap(*ptr);
		isInHeap[*ptr] = true;
	}
	TNode v = heap.PopHeap();
	isInHeap.erase(v);
	while (!heap.Empty()){
		for (auto ptr = a.GetNeighboursBegin(v); ptr != a.GetNeighboursEnd(v); ++ptr){
			if (isInHeap.count(*(*ptr).to) && (*ptr).weight < (*(*ptr).to).distance) {
				 (*(*ptr).to).distance = (*ptr).weight;
				 heap.Update(*(*ptr).to);
				 edges[*(*ptr).to] = *ptr;
			}
		}
		v = heap.PopHeap();
		isInHeap.erase(v);
		result.push_back(edges[v]);
	}
	return result;
}

