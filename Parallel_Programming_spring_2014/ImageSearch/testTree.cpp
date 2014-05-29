/*
 * =====================================================================================
 *
 *       Filename:  testTree.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  13.05.2014 16:59:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#include "KDTree.hpp"

class TestPoint {
public:
    TestPoint() {}
    TestPoint(int x, int y, int z) {
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(z);
    }

    TestPoint& operator=(const TestPoint& another) {
        vec = another.vec;
        return *this;
    }

    TestPoint(const TestPoint& another) {
        vec = another.vec;
    }

    int operator[](size_t indx) const {
        return vec[indx];
    }
    int Dimension() const {
        return vec.size();
    }
    std::vector<int> vec;
};

std::ostream& operator<<(std::ostream& o, const TestPoint& point) {
    o << "Point x:" << point.vec[0] << " y:" << point.vec[1] << " z:" << point.vec[2] << std::endl;
    return o;
}

int main() {
    std::vector<TestPoint> vec;
    for (int i = 0; i < 100; ++i) 
        vec.push_back(TestPoint(i, i + 1, i + 2));
    KDTree<TestPoint> tree (vec, 3);
    tree.PrintTree();
    std::cout << tree.GetNearestPoint(TestPoint(100, 100, 100)) << std::endl;
    return 0;
}

