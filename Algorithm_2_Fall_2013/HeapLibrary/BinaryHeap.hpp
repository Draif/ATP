/*
This Binary heap based on pointers, therefore it works slowly than based on vectors.
I made it to use with concurency. To watch usual BinaryHeap, go to HeapLibrary.h

(c) Pavel Iakovenko, 4th March 2014 
*/
#pragma once

template <class ptr>
class BasicHeapNode {
public:
    BasicHeapNode(): priority(0) {}
    ~BasicHeapNode() {}
    BasicHeapNode(long long pr) : priority(pr) {}
    BasicHeapNode(const BasicHeapNode& another) { *this = another; }
    BasicHeapNode& operator= (const BasicHeapNode& another) {
        priority = another.priority;
        parent = another.parent;
        leftChild = another.leftChild;
        rightChild = another.rightChild;
    }
    void SetPriority(long long newPr) {
        priority = newPr;
    }
    long long GetPriority() const {
        return priority;
    }
    bool operator<(const BasicHeapNode& another) {
        return priority < another.priority;
    }

private:
    long long priority;
    weak_ptr<ptr> parent;
    shared_ptr<ptr> leftChild;
    shared_ptr<ptr> rightChild;
};

template <class TNode>
class DefaultCmp {
public:
    bool operator()(TNode first, TNode second) {
        return first < second;
    }
};

template <class TNode, class cmp = DefaultCmp<TNode>>
class BinaryHeap {
public:
    BinaryHeap() {}
    long long Insert(const TNode&);
    TNode& GetTop();
    TNode ExtractTop();
    TNode Extract(long long);
    bool UpdatePriority(long long, long long); // return false if it can't be changed
private:
    void Heapify();
    shared_ptr<TNode> head;
    long long id;
    std::unordered_map<long long, shared_ptr<TNode>> indx;
};

template <class TNode>
long long BinaryHeap:: Insert (const TNode& newNode) {
    if (!head) {
        TNode* node = new TNode(newNode);
        heap = node;
    } else {





