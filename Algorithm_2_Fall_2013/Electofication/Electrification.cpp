#include <iostream>
#include "Graph.h"
#include "GraphAlgorithms.h"
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

struct TPoint:public node {
	int x, y;
};

//calculate distance between two points
double distanceCalculate(vector<TPoint>& points, unsigned int i, unsigned int j) {
	return sqrt((points[i].x - points[j].x)*(points[i].x - points[j].x) + (points[i].y - points[j].y) * (points[i].y - points[j].y));
}

int main () {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int n;
	cin >> n;
	vector <TPoint> points (n);
	for (int i = 0; i < n; ++i) {
		cin>> points[i].x >> points[i].y;
		points[i].index = i;
	}
	Graph <TPoint> graph (points);
	for (int i = 0; i < n; ++i) 
		for (int j = i +1; j < n; ++j) {
			double distance = distanceCalculate(points, i, j);
			TPoint* from = &graph.FindVertex(i);//terrible construction :(
			TPoint* to = &graph.FindVertex(j);
			graph.AddEdge(edge<TPoint>(from, to, distance)); 
			graph.AddEdge(edge<TPoint>(to, from, distance));
		}
	vector<edge<TPoint>> result = MST_PRIM<TPoint>(graph);
	double resultDistance = 0.0;
	for (auto ptr = result.begin(); ptr != result.end(); ++ptr) {
		resultDistance += ptr->weight;
	}
	cout << std::fixed << std::setprecision(3) <<	resultDistance;
	return 0;
}
