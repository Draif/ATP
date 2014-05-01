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

template < class TNode >
void TarjanStrongConnect ( Graph < TNode >& a, TNode* v, int* index, vector < TNode* >& S, vector < vector < TNode* > >& vec) {
	v->Tarjan_index = *index;
	v->Tarjan_low_index = *index;
	++*index;
	S.push_back ( v );
	for ( EdgeIterator<TNode> ptr = a.GetNeighboursBegin(*v);
			ptr != a.GetNeighboursEnd(*v); ++ ptr){
				if ( (*ptr).to->Tarjan_index == -1) {
					TarjanStrongConnect(a, (*ptr).to, index, S, vec);
					v->Tarjan_low_index = min (v->Tarjan_low_index, (*ptr).to->Tarjan_low_index);
				} else if ( find( S.begin(), S.end(), (*ptr).to) != S.end() )
					v->Tarjan_low_index = min (v->Tarjan_low_index, (*ptr).to->Tarjan_index);
	}

	if ( v->Tarjan_low_index == v->Tarjan_index) {
		vector < TNode* > temp;
		do {
			temp.push_back (S[S.size() - 1]);
			S.pop_back();
		} while ( temp [ temp.size() - 1] != v);
		vec.push_back(temp);
	}
}




template < class TNode>
vector < vector < TNode* > > Tarjan_SSC ( Graph < TNode >& a){
	a.ClearVertices();
	vector < TNode* > S;
	int index = 0;
	vector < vector < TNode* > > vec; 
	for ( VertexIterator < TNode> ptr = a.begin();
		ptr != a.end(); ++ptr)
		if ( (*ptr).Tarjan_index == -1)
			TarjanStrongConnect (a, &*ptr, &index, S,  vec);
	return vec;
}


