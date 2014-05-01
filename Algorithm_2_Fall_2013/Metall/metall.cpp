#include <iostream>
#include <stdio.h>
#include <vector>
#include <Graph.h>
#include <GraphAlgorithms.h>
using namespace std;

class NodeMetall: public node { //now this class created "just in case"
public:
    NodeMetall() {
    }
    NodeMetall(int i) {
        index = i;
    }
    std::vector<int> heightLess;
    std::vector<int> heightMore;
};

class EdgeMetall: public edge {
    int flow;
public:
    EdgeMetall() {
        flow = 0;
    }
    int GetFlow() const {
        return flow;
    }
    void SetFlow(int temp) {
        flow = temp;
    }
};


int minesNumber(int i) {
    return i + 2;
}

int factoriesNumber(int i) {
    return i - 2;
}

int main(int argc, char** argv) {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n, m;
    std::cin >> n >> m;
    std::vector<unsigned int> E (n); //production of mines 
    std::vector<unsigned int> P (m); //production of factories
    for (int i = 0; i < n; ++i) {
        std::cin >> E[i];
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> P[i];
    }
    std::vector<std::vector<unsigned int>> matrix (n);
    std::vector<unsigned int> temp (m); // to fullfil matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> temp[j];
        }
        matrix[i] = temp;
        temp.clear();
    }
    Graph<NodeMetall, EdgeMetall> graph;   
    NodeMetall f (1); //beginning
    for (int i = 0; i < n; ++i) {
        NodeMetall temp (minesNumber(i));
        graph.AddEdge(f, temp,  E[i]);
        graph.AddEdge(temp, f, -E[i]);
    }
    NodeMetall s (-1); //stock
    for (int i = 0; i < m; ++i) {
        NodeMetall temp (factoriesNumber(i));
        graph.AddEdge(temp, s, P[i]);
        graph.AddEdge(s, temp, -P[i]);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            graph.AddEdge(minesNumber(i), factoriesNumber(j), matrix[i][j]);
            graph.AddEdge(factoriesNumber(i), minesNumber(i), -matrix[i][j]);
        }
    }
    maximizeFlow(graph);
    return 0;
}
