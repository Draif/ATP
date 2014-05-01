#include "GraphAlgorithms.h"
#include "Graph.h"
#include <queue>

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

template <class TNode>
void DFS( Graph<TNode>& a){
	a.ClearVertices();
	int time = 0;
	for ( VertexIterator<TNode> ptr = a.begin();
		ptr != a.end(); ++ptr)
		if ( (*ptr).color == WHITE)
			DFS_VISIT(a, *ptr, &time);
}

template <class TNode>
void BFS (Graph <TNode> a, TNode& v){
	std:: queue <TNode*> q;
	a.ClearVertices();
	v.distance = 0;
	v.color = GREY;
	q.push(&v);
	TNode* temp;
	while ( ! q.empty){
		temp = q.top();
		q.pop();
		for ( EdgeIterator<TNode> ptr = a.GetNeighboursBegin(*temp);
			ptr != a.GetNeighboursEnd(*temp); ++ ptr)
			if ( (*ptr).color == WHITE ){
				*(ptr).parent = temp;
				*(ptr).distance = temp->distance + 1;
				*(ptr).color = GREY;
				q.push(ptr);
			}
		temp->color = BLACK;
	}
}

