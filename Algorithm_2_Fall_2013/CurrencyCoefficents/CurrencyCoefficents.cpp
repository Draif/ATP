#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <map>
#include "Graph.h"
using namespace std;


class CurrencyNode:public node{
public:
    CurrencyNode() {}
    CurrencyNode(int i) {
        index = i;
    }
    CurrencyNode(const CurrencyNode& another) {
        *this = another;
    }
    CurrencyNode& operator=(const CurrencyNode& another) {
        index = another.index;
    }
    bool operator==(const CurrencyNode& another) const {
        return index == another.index;
    }
    bool operator!=(const CurrencyNode& another) const {
        return !(*this == another);
    }
    int GetIndex() const {
        return index;
    }
    int index;
};

void PrintTrace(int startVertex, const vector<int>& parents) {
    vector<int> path;
    map<int, bool> visited;
    path.push_back(startVertex);
    bool flag = true;
    visited[startVertex] = true;
    for (;flag;) {
        int temp = parents[startVertex];
        path.push_back(temp);
        if (visited.find(temp) != visited.end()) {
            flag = false;
        }
        visited[temp] = true;
        startVertex = temp;
    }
    for (auto ptr = path.rbegin(); ptr != path.rend(); ++ptr) {
        cout << *ptr + 1<< " ";
    }
    cout << endl;
}

template<class TNode>
void FordBellman(Graph<TNode>& graph) {
    vector<double> distances (graph.size(), numeric_limits<double>::max());
    vector<int> parents (graph.size(), -1);
    int temp;
    distances[0] = 0.0;
    for (int i = 0; i < graph.size() - 1; ++i) {
        for (auto ptrVer = graph.begin(); ptrVer != graph.end(); ++ptrVer) {
            auto _end = graph.GetNeighboursEnd(*ptrVer);
            for (auto ptrEdge = graph.GetNeighboursBegin(*ptrVer); ptrEdge != _end; ++ptrEdge) {
                temp = (*(*ptrEdge).to).GetIndex();
                if (distances[(*ptrVer).GetIndex()] + (*ptrEdge).weight < distances[temp]) {
                    distances[temp] = distances[(*ptrVer).GetIndex()] + (*ptrEdge).weight;
                    parents[temp] = (*ptrVer).GetIndex();
                }
            }
        }
    }
    int startFrom;
    bool flag = false;
    for (auto ptrVer = graph.begin(); ptrVer != graph.end() && !flag; ++ptrVer) {
        auto _end = graph.GetNeighboursEnd(*ptrVer);
        for (auto ptrEdge = graph.GetNeighboursBegin(*ptrVer); ptrEdge != _end; ++ptrEdge) {
            temp = (*(*ptrEdge).to).GetIndex();
            if (distances[(*ptrVer).GetIndex()] + (*ptrEdge).weight < distances[temp]) {
                parents[temp] = (*ptrVer).GetIndex();
                startFrom = temp;
                flag = true;
                break;
            }
        }
    }
    if (flag) {
        PrintTrace(startFrom, parents);
    } else {
        cout << 0 <<endl;
    }
}



int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    cin >> n;
    vector<CurrencyNode> vec;
    for ( int i = 0; i < n; ++i) {
        vec.push_back(CurrencyNode(i));
    }
    Graph<CurrencyNode> graph(vec);
    double temp;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> temp;
            graph.AddEdge(vec[i], vec[j], -log(temp));
        }
    }
    FordBellman(graph);
    return 0;
}

