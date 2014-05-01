#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <Graph.h>
#include <GraphAlgorithms.h>
using namespace std;

struct MyNode : public node {
	int numberOfVariable;
	bool denial;
	int numberOfSSC;
	
	MyNode() {
	}

	MyNode(const MyNode& another) {
		*this = another;
	}
	MyNode& operator=(const MyNode& another) {
		numberOfVariable = another.numberOfVariable;
		denial = another.denial;
		numberOfSSC = another.numberOfSSC;
		index = another.index;
		return *this;
	}
};


unsigned int numberOfDenial (int number, int n) {
	if (number < 0)
		return -number;
	else 
		return number + n;
}

unsigned int numberOfDirect (int number, int n) {
	if (number < 0)
		return -number + n;
	else 
		return number;
}

bool canBeSolved (Graph<MyNode>& a, int n) {
	try {
		for (int i = 0; i < n; ++i)
			if (a.FindVertex(i+1).numberOfSSC == a.FindVertex(i+1 + n).numberOfSSC)
				return false;
	} catch (exception e){
		cout << e.what();
		exit(1);
	}
	return true;
}

void initializeVariables (Graph <MyNode>& a, int n) {
	for (int i = 0; i < n; ++i)
		if (a.FindVertex(i+1).numberOfSSC < a.FindVertex(i + 1 + n).numberOfSSC)
			cout << "1 ";
		else 
			cout <<"0 ";
}

	

int main () {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int n, m;
	cin >> n >> m;
	vector <MyNode> nodes (2 * n);
	for ( int i = 0; i < 2 * n; ++i) {
			nodes[i].index = i + 1;
		if ( i < n) 
			nodes[i].numberOfVariable = i +1;
		else
			nodes[i].numberOfVariable = i +1 - n;
		if ( i < n) 
			nodes[i].denial = false;
		else 
			nodes[i].denial = true;
	}
	Graph <MyNode> graph(nodes);
	for (int i = 0; i < m; ++i) { //really terible construction, which add edgesec
		int temp1, temp2;
		cin >> temp1 >> temp2;
		graph.AddEdge(nodes[numberOfDenial(temp1, n) - 1], nodes[numberOfDirect(temp2, n) - 1]);// denial 
		graph.AddEdge(nodes[numberOfDenial(temp2, n) - 1], nodes[numberOfDirect(temp1, n) - 1]);// denial 
	}
	Tarjan_SSC<MyNode, edge<MyNode>> (graph);
	if (!canBeSolved(graph, n)) {
		cout << "NO";
	} else {
		cout <<"YES\n";
		initializeVariables( graph, n);
	}
	return 0;
}
