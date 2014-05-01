#pragma once
#include <unordered_map>
#include <vector>
#include <utility>
#include <list>
#include <memory>
enum colors {WHITE, GREY, BLACK};

class node {
public:
	int index;
	int openTime;
	int closeTime;
	double distance;
	std::shared_ptr<node> parent;
	colors color;
	int Tarjan_index;
	int Tarjan_low_index;
	node() {
	}
	node(const node& another) {
		*this = another;
	}
	node(int n ){
		index = n;
	}
	virtual void clear() {
		openTime = closeTime = 0;
		Tarjan_index = Tarjan_low_index = -1;
		distance = 0;
		parent.reset();
	}
	bool operator< ( const node& another) const {
		return index < another.index;
	}
	bool operator== ( const node another) const {
		return index == another.index;
	}
	bool operator!= ( const node& another) const {
		return ! ( *this == another);
	}
	virtual node& operator=(const node& another) {
		index = another.index;
		openTime = another.openTime;
		closeTime = another.closeTime;
		distance = another.distance;
		parent = another.parent;
		Tarjan_index = another.Tarjan_index;
		Tarjan_low_index = another.Tarjan_low_index;
		return *this;
	}
};

template <class TNode>
class edge {
public:
	double weight;
	int color;
	// as if pointers here points to objects in unordered_map, it is wrong here to use shared_ptr
	TNode* from;
	TNode* to;

	edge () {
		from = to = NULL;
		weight = 0;
	}
	~edge() {
	}

	edge (TNode* f, TNode* t){
		from = f;
		to = t;
	}

	edge (TNode* f, TNode* t, double w){
		from = f;
		to = t;
		weight = w;
	}

	bool operator==(const edge& another) {
		return (weight == another.weight && color == another.color &&
			from == another.from && to == another.to);
	}
};

template <class TNode, class TEdge>
class ConstVertexIterator{
	typedef typename std::unordered_map<int, std::pair<TNode, std::list<TEdge>>>::const_iterator Iter;
	Iter ptr;
public:
	ConstVertexIterator (Iter& temp){
		ptr = std::move(temp);
	}
	
	const TNode operator* (){
		return (*ptr).first;
	}

	bool operator== (const ConstVertexIterator<TNode, TEdge>& another) const {
		return ptr == another.ptr;
	}

	bool operator!= ( const ConstVertexIterator<TNode, TEdge>& another) const {
		return ! (*this == another);
	}

	const ConstVertexIterator& operator++ (){
		++ptr;
		return *this;
	}
};

template <class TNode, class TEdge>
class VertexIterator{
	typedef	typename std::unordered_map<int, std::pair<TNode, std::list<TEdge>>>::iterator Iter;
	Iter ptr;
public:
	VertexIterator (Iter temp){
		ptr = std::move(temp);
	}
	
	VertexIterator (const VertexIterator<TNode, TEdge>& a){
		ptr = a.ptr;
	}
	
	TNode& operator* (){
		return (*ptr).second.first;
	}

	TNode* operator->() {
		return &ptr->second.first;;
	}

	bool operator==(const VertexIterator<TNode, TEdge>& another) const {
		return ptr == another.ptr;
	}

	bool operator!=( const VertexIterator<TNode, TEdge>& another) const {
		return ! (*this == another);
	}

	VertexIterator& operator++ (){
		++ptr;
		return *this;
	}
};

template <class TEdge>
class ConstEdgeIterator{
	typedef typename std::list<TEdge>::const_iterator Iter;
	Iter ptr;
public:
	ConstEdgeIterator(Iter temp){
		ptr = std::move(temp);
	}

	ConstEdgeIterator(ConstEdgeIterator<TEdge>& another){
		ptr = another.ptr;
	}
	const TEdge& operator*(){
		return *ptr;
	}
	bool operator==(const ConstEdgeIterator<TEdge>& another){
		return (ptr == another.ptr);
	}
	
	bool operator!=(const ConstEdgeIterator<TEdge>& another){
		return ! (*this == another);
	}
	const ConstEdgeIterator<TEdge>& operator++ (){
		++ptr;
		return *this;
	}
};

template <class TEdge>
class EdgeIterator{
	typedef typename std:: list<TEdge> :: iterator Iter; 
	Iter ptr;
public:
	EdgeIterator(Iter temp){
		ptr = std::move(temp);
	}
	EdgeIterator ( const EdgeIterator<TEdge>& another){
		ptr = another.ptr;
	}
	TEdge& operator*(){
		return *ptr;
	}
	
	TEdge* operator->(){
		return &(*ptr);
	}

	bool operator==(const EdgeIterator<TEdge>& another){
		return (ptr == another.ptr);
	}
	
	bool operator!=(const EdgeIterator<TEdge>& another){
		return ! (*this == another);
	}
	EdgeIterator<TEdge>& operator++ (){
		++ ptr;
		return *this;
	}
};

template <class TNode, class TEdge = edge<TNode>>
class Graph{
	std:: unordered_map<int, std::pair<TNode, std:: list<TEdge>>> vertex;
	typedef typename std::unordered_map<int, std::pair<TNode, std:: list<TEdge>>>::iterator Iter;
	typedef typename std::unordered_map<int, std::pair<TNode, std:: list<TEdge>>>::const_iterator ConstIter;
	typedef typename std::list<TEdge>::const_iterator ConstListIter;

	int IfVertexExists (const TNode& a) const {
		return vertex.count(a.index);
	}

public:
	Graph() {
		TNode a;
		TEdge b;
		node* check1 = dynamic_cast<node*>(&a); // check, whether TNode is derived from node
	    //edge<node>* check2 = dynamic_cast<edge<TNode>*>(&b); // check, whether TEdge is derived from edge
	}

	Graph(std::vector <TNode>& vec) {
		TNode a;
		TEdge b;
		node* check1 = dynamic_cast<node*>(&a); // check, whether TNode is derived from node
		//edge<node>* check2 = dynamic_cast<edge<node>*>(&b); // check, whether TEdge is derived from edge	
		for (typename std::vector<TNode>::iterator ptr = vec.begin();
			ptr != vec.end(); ++ptr) {
				if (vertex.find(ptr->index) == vertex.end()) {
						TNode temp = *ptr;
						std:: list <TEdge> emptyList;
						vertex[ptr->index] = std::make_pair<TNode, std:: list <TEdge>>(std::move(temp), std::move(emptyList));
				}
		}
	}

	void AddVertex(const TNode& a) {
		Iter element = vertex.find(a.index);
		if (element == vertex.end()) {
			std::list<TEdge> emptyList;
			TNode temp = a;
			vertex[a.index] = std::make_pair<TNode, std:: list <TEdge>>(std::move(temp), std::move(emptyList));
		}
	}

	void AddEdge(const TNode& a, const TNode& b) {
		int vertexOne = vertex.count(a.index);
		int vertexTwo = vertex.count(b.index);
		if ( vertexOne != 0 && vertexTwo  != 0){
			vertex[a.index].second.push_back(TEdge(&vertex[a.index].first, &vertex[b.index].first));
		} else {
			if ( vertexOne == 0 ){
				AddVertex(a);
			}
			if ( vertexTwo == 0 ){
				AddVertex(b);
			}
			vertex[a.index].second.push_back(TEdge(&vertex[a.index].first, &vertex[b.index].first));
		}
	}
    void AddEdge(const TNode& a, const TNode& b, double weight) {
        int vertexOne = vertex.count(a.index);
        int vertexTwo = vertex.count(b.index);
        if (vertexOne != 0 && vertexTwo != 0) {
            vertex[a.index].second.push_back(TEdge(&vertex[a.index].first, &vertex[b.index].first, weight));
        } else {
            if (vertexOne == 0) {
                AddVertex(a);
            }
            if (vertexTwo == 0) {
                AddVertex(b);
            }
            vertex[a.index].second.push_back(TEdge(&vertex[a.index].first, &vertex[b.index].first, weight));
        }
    }


	void AddEdge ( const TEdge& Edge){
		 const edge<TNode>* check2 = dynamic_cast<const edge<TNode>*>(&Edge); // check, whether TEdge is derived from edge
		int vertexOne = IfVertexExists(*Edge.from);
		if (vertexOne == 0) {
			AddVertex(*(Edge.from));
		}
		vertex[Edge.from->index].second.push_back(Edge);
	}

	bool HasEdge ( const TNode& a, const TNode& b){
		int vertexOne = IfVertexExists(a);
		if (vertexOne != 0) {
			ConstListIter end = vertex[a.index].second.cend();
			for (ConstListIter ptr = vertex[a.index].second.cbegin(); ptr != end; ++ptr)
				if (*ptr = b)
					return true;
		}
		return false;
	}

	TNode&  FindVertex(int index) {
	    return vertex.at(index).first;
	}

	unsigned int size () const {
		return vertex.size();
	}

	void ClearVertexes() {
		for (Iter ptr = vertex.begin(); ptr != vertex.end(); ++ptr) {
			ptr->second.first.clear();
		}
	}

	VertexIterator<TNode, TEdge> Find (const TNode& a){
		VertexIterator<TNode, TEdge> temp (vertex.find(a.index));
		return temp;
	}

	ConstVertexIterator<TNode, TEdge> cbegin() const{
		return ConstVertexIterator<TNode, TEdge> (vertex.cbegin());
	}
	ConstVertexIterator<TNode, TEdge> cend() const{
		return ConstVertexIterator<TNode, TEdge> (vertex.cend());
	}

	VertexIterator<TNode, TEdge> begin() {
		return VertexIterator<TNode, TEdge> (vertex.begin());;
	}
	VertexIterator<TNode, TEdge> end(){
		return VertexIterator<TNode, TEdge>(vertex.end());
	}
	
	ConstEdgeIterator<TEdge> GetNeighboursBeginConst( const TNode& a) const{
	    return ConstEdgeIterator<TEdge> (vertex.at(a.index).second.cbegin());
	}

	ConstEdgeIterator<TEdge> GetNeighboursEndConst (const TNode& a) const{
		return ConstEdgeIterator<TEdge> (vertex.at(a.index).second.cend());
	}

	EdgeIterator<TEdge> GetNeighboursBegin(const TNode& a){
		return EdgeIterator<TEdge> (vertex.at(a.index).second.begin());
	}

	EdgeIterator<TEdge> GetNeighboursEnd (const TNode& b){
		return EdgeIterator<TEdge> (vertex.at(b.index).second.end());
	}

};
