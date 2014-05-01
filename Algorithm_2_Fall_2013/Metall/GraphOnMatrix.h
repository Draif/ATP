#pragma once
#include <iostream>
#include <vector>

class edgeMetall {
    int weight;
    int flow;
public:
    edgeMetall() {
        weight = 0;
    }
    edgeMetall(int _weight) {
        weight = _weight;
    }
    int GetWeight() const {
        return weight;
    }
    int GetFlow() {
        return flow;
    }
    void SetFlow(int newFlow) {
        flow = newFlow;
    }
};

class nodeMetall {
public:
    std::vector<int> less; // with lessHeigh
    std::vector<int> moreOrEqual; 
    bool hasLessHeight() {
        return !less.empty();
    }
    void changeNode(int number) {
        less.erase(number);
        moreOrEqual.push_back(number);
    }

};

class GraphForMetall {
    std::vector<nodeMetall> factories;
    std::vector<edgeMetall> mines;
    std::vector<std::vector<edgeMetall> > matrix; //mines on vertical, factories - horiz.
public:
    GraphForMetall(unsigned int minesNumber, unsigned int factoriesNumber) {
        factories.resize(factoriesNumber);
        mines.resize(minesNumber);
        matrix.resize(minesNumber + 2, std::vector<edgeMetall> (factoriesNumber + 2)); 
    }
    //minesNumber and factoriesNumber start from 1 !
    edgeMetall& GetEdge(unsigned int minesNumber, unsigned int factoriesNumber) {
        return matrix[minesNumber][factoriesNumber];
    }
    nodeMetall& GetFactoryNode(unsigned int factory) {
        return factories[factory];
    }
    nodeMetall& GetMineNode(unsigned int mine) {
        return mines[mine];
    }
    void SetInitialMineProductivity (unsigned int mine, unsigned int productivity) {
        matrix[mine][0] = edgeMetall(productivity);
    }
    void SetInitialFactoryProductivity(unsigned int factory, unsigned int productivity) {
        matrix[matrix.size() - 1][factory] = edgeMetall(productivity);
    }
};

