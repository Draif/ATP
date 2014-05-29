/*
 * =====================================================================================
 *
 *       Filename:  K-DTree.hpp
 *
 *    Description:  Contain Parallel K-d tree
 *
 *        Version:  1.0
 *        Created:  13.05.2014 14:52:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>
#include <future>
#include <mutex>
#include <functional>


template <class Point>
class KDNode {
    typedef std::shared_ptr<KDNode<Point>> Ptr; 
public:
    KDNode() : dividingDimension(0), isLeaf(true) {}
    
    KDNode(const Point& p, size_t divDimension) : point(p), dividingDimension(divDimension), isLeaf(true) {}
    
    KDNode(const KDNode& node) {
        leftChild = node.leftChild;
        rightChild = node.rightChild;
        point = node.point;
        dividingDimension = node.dividingDimension;
        isLeaf = node.isLeaf;
    }

    KDNode<Point>& operator=(const KDNode<Point>& node) {
        leftChild = node.leftChild;
        rightChild = node.rightChild;
        point = node.point;
        dividingDimension = node.dividingDimension;
        isLeaf = node.isLeaf;
        return *this;
    }
    
    const Point& GetPoint() const {
        return point;
    }

    Point& GetPoint() {
        return point;
    }

    void SetPoint(const Point& _point) {
        point = _point;
    }
    
    KDNode& CreateLeftChild() {
        if (!leftChild)
            leftChild = std::make_shared<KDNode<Point>>(KDNode<Point>());
        return *leftChild;
    }

    KDNode& CreateRightChild() {
        if (!rightChild)
            rightChild = std::make_shared<KDNode<Point>>(KDNode<Point>());
        return *rightChild;
    }

    void CreateLeftChild(const KDNode<Point> node) {
        leftChild = std::make_shared<KDNode<Point>>(node);
    }

    void CreateRightChild(const KDNode<Point> node) {
        rightChild = std::make_shared<KDNode<Point>>(node);
    }
    
    void SetDividingDimension(size_t d) {
        dividingDimension = d;
    }

    size_t GetDividingDimension() const {
        return dividingDimension;
    }

    bool IsLeaf() const {
        return isLeaf;
    }

    void SetLeaf(bool flag) {
        isLeaf = flag;
    }   

    void Print() {
        point.PrintInformation();
        if (leftChild)
            leftChild->Print();
        if (rightChild)
            rightChild->Print();
    }

    bool HasRightChild() const {
        return static_cast<bool>(rightChild);
    }

    bool HasLeftChild() const {
        return static_cast<bool>(leftChild);
    }

    const KDNode<Point>& GetRightChild() const {
        return *rightChild;
    }

    const KDNode<Point>& GetLeftChild() const {
        return *leftChild;
    }
    
    KDNode<Point>& GetRightChild() {
        return *rightChild;
    }

    KDNode<Point>& GetLeftChild() {
        return *leftChild;
    }
private:
    Ptr leftChild;
    Ptr rightChild;
    Point point;
    size_t dividingDimension;
    bool isLeaf;
};

template <class Point>
class EuclidDistance {
public:
    double operator()(const Point& a, const Point& b) {
        if (a.Dimension() != b.Dimension())
            throw std::runtime_error("Dimensions is not equal!");
        double res = 0.0;
        for (int i = 0; i < a.Dimension(); ++i)
            res += pow(static_cast<double>(a[i]) - static_cast<double>(b[i]), 2.0);
        return sqrt(res);
    }
};

template <class Point, class Distance = EuclidDistance<Point>>
class KDTree {
    typedef typename std::vector<Point>::iterator Iter;
public:
    KDTree(size_t dim) : dimension(dim), empty(true) {}

    KDTree(std::vector<Point> vec, size_t dim) : dimension(dim) {
        if (vec.empty())
            empty = true;
        else {
            BuildTree(vec.begin(), vec.end(), root);
            empty = false;
        }
    }

    Point GetNearestPoint(const Point& point) {
        double currentBest = -1;
        KDNode<Point> nearestNode;
        NearestNode(root, point, currentBest, nearestNode);
        return nearestNode.GetPoint();
    }
    
    void AddPoint(const Point& point) {
        if (!empty)
            InsertNode(root, point);
        else {
            root = KDNode<Point>(point, 0);
            empty = false;
        }
    }
    
    void Build(std::vector<Point> vec) {
        std::cout << "vec size: " << vec.size() << std::endl;
        if (vec.empty())
            empty = true;
        else {
            BuildTree(vec.begin(), vec.end(), root);
            empty = false;
        }
    }

    void PrintTree() {
        root.Print();
    }

private:
    void BuildTree(Iter beg, Iter end, KDNode<Point>& node, size_t depth = 0) {
        size_t dist = std::distance(beg, end);
        if (dist == 0) {
            return;
        }
        size_t axis = depth % dimension;

        auto comp = [=](const Point& a, const Point& b) {
            return a[axis] < b[axis];
        };

        std::sort(beg, end, comp);
        size_t median = dist / 2;
        node.SetPoint(*(beg + median));
        node.SetDividingDimension(axis);
        node.GetPoint().PrintInformation();
        node.SetLeaf(false);
        if (dist != 1) {
            std::future<void> fut;
            if (beg + median != beg) {
                fut = std::async(&KDTree<Point, Distance>::BuildTree, this, beg, beg + median, std::ref(node.CreateLeftChild()), depth + 1);
            }
            if (beg + median + 1 != end)
                BuildTree(beg + median + 1, end, node.CreateRightChild(), depth + 1);
            if (fut.valid())
                fut.get();
        } else 
            node.SetLeaf(true);
    }

    int NearestNode(KDNode<Point>& node, const Point& point, double& currentBest, 
                    KDNode<Point>& nearestPoint) {
        if (node.IsLeaf()) {
            double distance = operDistance(node.GetPoint(), point);
            if (currentBest == -1 || distance < currentBest) {
                currentBest = distance;
                nearestPoint = node;
            }
            return 1;                 
        } else {
            double distance = operDistance(node.GetPoint(), point);
            size_t axis = node.GetDividingDimension();
            if (node.GetPoint()[axis] < point[axis] && node.HasRightChild()) {
                std::future<int> fut;
                if (node.HasLeftChild()) {
                    fut = std::async(&KDTree<Point, Distance>::NearestNode, this,  std::ref(node.GetLeftChild()), std::ref(point), std::ref(currentBest), std::ref(nearestPoint));
                }
                NearestNode(node.GetRightChild(), point, currentBest, nearestPoint);
                if (fut.valid())
                    fut.get();
            } else {
                std::future<int> fut;
                if (node.HasRightChild()) {
                    fut = std::async(&KDTree<Point, Distance>::NearestNode, this, std::ref(node.GetRightChild()), std::ref(point), std::ref(currentBest), std::ref(nearestPoint));
                }
                NearestNode(node.GetLeftChild(), point, currentBest, nearestPoint);
                if (fut.valid())
                    fut.get();
            }
            mtx.lock();
            if (currentBest == -1 || distance < currentBest) {
                currentBest = distance;
                nearestPoint = node;
            }
            mtx.unlock();
        }
        return 1;
    }
    
    void InsertNode(KDNode<Point>& node, const Point& point) {
        size_t axis = node.GetDividingDimension();
        if (node.IsLeaf()) {
            if (node.GetPoint()[axis] < point[axis]) 
                node.CreateRightChild(KDNode<Point>(point, (axis + 1) % dimension));
            else 
                node.CreateLeftChild(KDNode<Point>(point, (axis + 1) % dimension));
            node.SetLeaf(false);
        } else 
            if (node.GetPoint()[axis] < point[axis]) 
                if (node.HasRightChild())
                   InsertNode(node.GetRightChild(), point); 
                else 
                    node.CreateRightChild(KDNode<Point>(point, (node.GetDividingDimension() + 1) % dimension));
            else 
                if (node.HasLeftChild())
                   InsertNode(node.GetLeftChild(), point); 
                else 
                    node.CreateLeftChild(KDNode<Point>(point, (node.GetDividingDimension() + 1) % dimension));
    }
    std::mutex mtx;
    bool empty;
    KDNode<Point> root;     
    Distance operDistance;
    size_t dimension;
};


