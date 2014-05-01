using namespace std;

enum colors {WHITE, GREY, BLACK};

struct node {
	int index;
	int openTime;
	int closeTime;
	int distance;
	node* parent;
	colors color;
	int Tarjan_index;
	int Tarjan_low_index;
	node () {
	}
	node ( int n ){
		index = n;
	}
	bool operator< ( const node& another) const {
		return index < another.index;
	}
	bool operator== ( const node another)const {
		return index == another.index;
	}
	bool operator!= ( const node& another) const {
		return ! ( *this == another);
	}
};

template <class TNode>
struct edge {
	int weight;
	int color;
	TNode* from;
	TNode* to;

	edge (TNode* f, TNode* t){
		from = f;
		to = t;
	}

	bool operator== ( const edge& another){
		return (weight == another.weight && color == another.color &&
			from == another.from && to == another.to);
	}
};

template <class TNode >
class ConstVertexIterator{
	typename  vector <TNode> ::  const_iterator ptr;
public:
	ConstVertexIterator ( typename  vector <TNode> :: const_iterator& temp){
		ptr = temp;
	}
	
	const TNode operator* (){
		return *ptr;
	}

	bool operator== (const ConstVertexIterator<TNode>& another) const {
		return ptr == another.ptr;
	}

	bool operator!= ( const ConstVertexIterator<TNode>& another) const {
		return ! (*this == another);
	}

	const ConstVertexIterator& operator++ (){
		++ ptr;
		return *this;
	}
};

template <class TNode >
class VertexIterator{
	typename vector< TNode> ::  iterator ptr;
public:
	VertexIterator (  typename vector < TNode > ::  iterator temp){
		ptr = temp;
	}
	
	VertexIterator ( VertexIterator& a){
		ptr = a.ptr;
	}
	
	TNode& operator* (){
		return *ptr;
	}

	bool operator== (const VertexIterator<TNode>& another) const {
		return ptr == another.ptr;
	}

	bool operator!= ( const VertexIterator<TNode>& another) const {
		return ! (*this == another);
	}

	VertexIterator& operator++ (){
		++ ptr;
		return *this;
	}
};

template <class TNode, class TEdge = edge < TNode> >
class ConstEdgeIterator{
	typename list<TEdge> :: const_iterator ptr;
public:
	ConstEdgeIterator( typename list<TEdge> :: const_iterator temp){
		ptr = temp;
	}

	ConstEdgeIterator ( ConstEdgeIterator& another){
		ptr = another.ptr;
	}
	const TEdge& operator*(){
		return *ptr;
	}
	bool operator== (const ConstEdgeIterator<TEdge>& another){
		return (ptr == another.ptr);
	}
	
	bool operator!= (const ConstEdgeIterator<TEdge>& another){
		return ! (*this == another);
	}
	const ConstEdgeIterator<TNode>& operator++ (){
		++ ptr;
		return *this;
	}
};

template <class TNode, class TEdge = edge < TNode> >
class EdgeIterator{
	typename list<TEdge> :: iterator ptr;
public:
	EdgeIterator( typename list<TEdge> :: iterator temp){
		ptr = temp;
	}
	EdgeIterator ( EdgeIterator& another){
		ptr = another.ptr;
	}
	TEdge& operator*(){
		return *ptr;
	}
	
	TEdge* operator->(){
		return &(*ptr);
	}

	bool operator== (const EdgeIterator<TNode,TEdge>& another){
		return (ptr == another.ptr);
	}
	
	bool operator!= (const EdgeIterator<TNode, TEdge>& another){
		return ! (*this == another);
	}
	EdgeIterator<TNode, TEdge>& operator++ (){
		++ ptr;
		return *this;
	}
};

template <class TNode, class TEdge = edge<TNode> >
class Graph{
	vector <TNode> v;
	vector <list < TEdge > > adj;

	int FindVertex (const TNode& a) const {
		unsigned int i;
		for (i =0; i < v.size(); ++i)
			if ( v[i] == a)
				break;
		return (i != v.size()) ? i : -1;
	}

public:
	Graph (){
		index = 0;
	}

	Graph( vector < TNode >& vec){
		list < TEdge > a;
		for ( vector <TNode> :: iterator ptr = vec.begin();
			ptr != vec.end(); ++ptr){
				ptr->index = v.size();
				v.push_back(*ptr);
				adj.push_back(a);
		}
	}

	Graph ( int n){
		v.resize(n);
		adj.resize(n);
	}

	void AddVertex ( const TNode& a){
		vector <TNode> :: iterator ptr = find(v.begin(), v.end(), a);
		if ( ptr == v.end()){
			v.push_back(a);
			list <TEdge> l;
			adj.push_back( l);
		}
	}

	void AddEdge ( const TNode& a, const TNode& b){
		int vertexOne = FindVertex(a);
		int vertexTwo = FindVertex(b);
		if ( vertexOne != -1 && vertexTwo  != -1){
			TEdge temp (&v[vertexOne],&v[vertexTwo]);
			adj[vertexOne].push_back(temp);
		} else {
			if ( vertexOne == -1 ){
				AddVertex(a);
				vertexOne = v.size() - 1;
			}
			if ( vertexTwo == -1 ){
				AddVertex(b);
				vertexTwo = v.size() - 1;
			}
			TEdge temp (&v[vertexOne],&v[vertexTwo]);
			adj[vertexOne].push_back(temp);
		}
	}

	bool HasEdge ( const TNode& a, const TNode& b){
		int vertexOne = FindVertex(a);
		if ( vertexOne == -1 )
			return false;
		else {
			list < TEdge > :: iterator ptr = adj[vertexOne].begin();
			for ( ; ptr!= adj[vertexOne].end(); ++ptr)
				if ( *((*ptr).to) == b)
					break;
			return ( ptr != adj[vertexOne].end() )? true : false;
		}
	}

	void ClearVertices () {
		for (unsigned int i = 0; i < v.size(); ++i){
				v[i].color = WHITE;
				v[i].openTime = 0;
				v[i].closeTime = 0;
				v[i].distance = -1;
				v[i].parent = NULL;
				v[i].Tarjan_index = -1;
				v[i].Tarjan_low_index = -1;
		}
	}

	VertexIterator<TNode> Find (const TNode& a){
		typename vector<TNode> :: iterator ptr = v.begin();
		for ( ;( ptr != v.end() && *ptr != a); ++ptr);
			return VertexIterator<TNode> (ptr);
	}

	ConstVertexIterator<TNode> cbegin() const{
		return ConstVertexIterator<TNode> (adj.cbegin());
	}
	ConstVertexIterator<TNode> cend() const{
		return ConstVertexIterator<TNode> (adj.cend());
	}

	VertexIterator<TNode> begin(){
		return VertexIterator<TNode> (v.begin());
	}
	VertexIterator<TNode> end(){
		return VertexIterator<TNode> (v.end());
	}
	
	ConstEdgeIterator<TNode> GetNeighboursBeginConst( const TNode& a) const{
		return ConstEdgeIterator<TNode> (adj[a].cbegin());
	}

	ConstEdgeIterator<TNode> GetNeighboursEndConst (const TNode& b) const{
		return ConstEdgeIterator<TNode> (adj[a].cend());
	}

	EdgeIterator<TNode> GetNeighboursBegin( const TNode& a){
		return EdgeIterator<TNode> (adj[FindVertex(a)].begin());
	}

	EdgeIterator<TNode> GetNeighboursEnd (const TNode& b){
		return EdgeIterator<TNode> (adj[FindVertex(b)].end());
	}

};
