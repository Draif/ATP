#include <iostream>
#include "GraphOnMatrix"
usign namespace std;

int start() {
    return 1;
}

int finish() {
    return -1;
}

int mine(int mineNumber) {
    return mineNumber + 2;
}

int factory(int factoryNumber) {
    return -factoryNumber - 2;
}

nodeMetall& returnNodeByNumber(int numb, const GraphForMetall& graph) {
    if (numb < 0) {
        return graph.GetFactoryNode(-(numb + 2));
    } else {
        return graph.GetMineNode(numb - 2);
    }
}

int main() {
    vector<int> overLoaded;
    unordered_map<int> height;
    unordered_map<int> e;
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n, m;
    cin >> n >> m;
    vector<int> initialProductivityOfMines (n);
    vector<int> initialProductivityOfFactories (m);
    GraphforMetall graph (n, m);
    for (int i = 0; i < n; ++i) {
        cin >> initialProductivityOfMines[i];
        graph.SetInitialMineProductivity(i + 1, initialProductivityOfMines[i]);
    overLoaded.push_back(mine(i));
    }
    for (int i = 0; i < m; ++i) {
        cin >> initialProductivityOfFactories[i];
        graph.SetInitialFactoryProductivity(i + 1, initialProductivityOfFactories[i]);
    }
    e[start()] = height[start()] = n + m + 1;
    e[finish()] = height[finish()] = 0;
    for (int i = 0; i < n; ++i) {
        e[mine(i)] = height[mine(i)] = 0;
    }
    for (int i = 0; i < m; ++i) {
        e[factory(i)] = height[mine(i)] = 0;
    }
    while(!overLoaded.empty()) {
        int nodeNumber = overLoaded,back();
        nodeMetall u = returnNodeByNumber(nodeNumber);
        if (u.hasLessHeight()) { //relable
            int oldHeight = height[nodeNumber]; 
            int minHeight;
            for (int i = 0; i < u.moreOrEqual.size(); ++i) {
                if (i == 0) {
                    minHeight = height[u.moreOrEqual[i]];
                } else if (minHeight > height[u.moreOrEqual[i]]) {
                    minHeight = height[u.moreOrEqual[i]];
                }
            }
            height[nodeNumber] = 1 + minHeight;
            for (int i = 0; i < u.moreOrEqual.size(); ++i) {
                if (height[u.moreOrEqual[i]] < height[nodeNumber]) {
                    u.less.push_back(u.moreOrEqual[i]);
                    u.moreOrEqual.erase(u.moreOrEqual.begin() + i);
                    --i;
                }
            }
            if (nodeNumber < 0) {
                for (int i = 0; i < n; ++i) {
                    if ( oldheight < height[mine(i)] && height[mine(i)] <= height[nodeNumber])
                        graph.GetMine(i).changeNode(nodeNumber);
                }
            } else {
                for (int i = 0; i < m; ++i) {
                    if ( oldheight < height[factory(i)] && height[factory(i)] <= height[nodeNumber])
                        graph.GetFactory(i).changeNode(nodeNumber);
                }
            }
        } else {
            push(u);
        }
    }
    for (int i = 1; i <= n; ++i) {
        for ( int j = 1; j <= m; ++j) {
            cout << graph.GetEdge(i, j).GetFlow(); << " ";
        }
        cout << std::endl;
    }
    return 0;
}




    
         
         
