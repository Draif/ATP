#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Graph.h"
#include "GraphAlgorithms.h"
using namespace std;




 int main() {
	/* vector <node> v;
	 for ( int i = 0; i < 5; ++i)
		 v.push_back( node ( i+1 ) );
	 Graph<node> a (v);
	 node b (6);
	 a.AddVertex( b);
	 for (int i = 1; i < 5; ++i)
		 a.AddEdge(v[0], v[i]);
	 bool flag = a.HasEdge(v[0], v[1]);
	 EdgeIterator<node> ptr = a.GetNeighboursBegin(v[0]);
	 for (; ptr != a.GetNeighboursEnd(v[0]); ++ptr)
		 (*ptr).to->color = GREY;
	 a.ClearVertexes();*/
	 int n, m;
	 cin >> n >> m;
	 vector < node> vec (n);
	 Graph <node> a (vec);
	 for (int i = 0; i < m; ++i){
		 int from, to;
		 cin >> from >> to;
		 a.AddEdge(vec[from -1], vec [ to -1]);
	 }
	 DFS(a);
	 BFS <node>(a, vec[0]);
	 Tarjan_SSC(a);
	 return 0;
 }