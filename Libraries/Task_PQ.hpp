#pragma once
#include <vector>
#include <utility>  
#include <queue>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/atomic.hpp>

template <class TTask>
class MyComp {
public:
    bool operator()(const TTask& a, const TTask& b) const {
        return a > b;
    }
};

template <class Node> 
class IConcurentQueue {
    virtual void put(const Node&) = 0;
    virtual bool pop(Node&) = 0;
    virtual bool empty() const = 0;
    virtual void finishWork() = 0;
};

// usual parallel queue 
template <class Node>
class ConcurentQueue : public IConcurentQueue<Node> {
    typedef std::lock_guard<std::mutex> guard;
public:
    void put(const Node& task) {
        guard lk(mtx);
        dataQueue.push(task);
    }

    //returns true if pop is seccessful
    bool pop(Node& val) {
        std::unique_lock<std::mutex> lk(mtx);
        if (dataQueue.empty())
            return false;
        val = std::move(dataQueue.front());
        dataQueue.pop();
        return true;
    }

    bool empty() const {
        guard lk(mtx);
        return dataQueue.empty();
    }

    void finishWork() {}

private:
    std::queue<Node> dataQueue;
    mutable std::mutex mtx;
};

template <class Node, class Compare = MyComp<Node>>
class ConcurrentPQ : public IConcurentQueue<Node> {
    typedef boost::detail::spinlock SpinLock;
public:
    ConcurrentPQ(): done(false) {
    }

    void finishWork() {
        done = true;
        cv.notify_all();
    }

    void put(const Node& task) {
        lockMtx.lock();
        boost::upgrade_lock<boost::shared_mutex> lock (mtx);
        lockMtx.unlock();
        boost::upgrade_to_unique_lock<boost::shared_mutex> uniquelock (lock);
        array.emplace_back(std::make_pair(task, SpinLock()));
        array.back().second.unlock();
        ShiftUp(array.size() - 1);
    }

     bool pop(Node& node) {
        lockMtx.lock();
        boost::upgrade_lock<boost::shared_mutex> ulock (mtx);
        lockMtx.unlock();
        if (empty()) {
            return false;
        }
        auto top = GetLock(0);
        {   boost::upgrade_to_unique_lock<boost::shared_mutex> uniquelock (ulock);
            node  = array.front().first;
            if (array.size() > 1) {
                auto last = GetLock(array.size() - 1);
                array.front().first = array.back().first;
                last.unlock();
            }
            array.pop_back();
        }
        ulock.unlock();
        if (array.size() > 1) {
            ShiftDown(0, std::move(top));
        } 
        return true;
    }

    bool empty() const {
        ReadLock lock (mtx);
        if (array.size() == 0) {
            return true;
        }
        return false;
    }
    
private:
    boost::mutex lockMtx;
    typedef boost::shared_lock<boost::shared_mutex> ReadLock;
    typedef boost::unique_lock<boost::shared_mutex> WriteLock;
    boost::atomic_bool done;
    boost::shared_mutex mtx;
    boost::mutex pop_mtx;
    std::vector<std::pair<Node,SpinLock>> array;
    Compare comp;
    boost::condition_variable cv;
    size_t Parent(size_t i) {
        return (i-1)/2;
    }
    size_t LeftChild(size_t i) {
        return 2*i + 1;
    }
    size_t RightChild(size_t i) {
        return 2*i + 2;
    }
    bool ThreadWait() {
        boost::unique_lock<boost::mutex> lock (pop_mtx);
        cv.wait(lock, [this]()->bool { return (array.size() > 0 || done);});
        if (done) 
            return false;
        return true;
    }

    std::unique_lock<SpinLock> GetLock(size_t index, bool defer = false) {
        if (defer) 
            return std::unique_lock<SpinLock>(array[index].second, std::defer_lock);
        else 
            return std::unique_lock<SpinLock>(array[index].second);
    }

    void ShiftUp(size_t i) {
        while(i > 0 &&  comp(array[i].first, array[Parent(i)].first)) {
            std::swap(array[i].first, array[Parent(i)].first);
            i = Parent(i);
        }
    }
    void ShiftDown(size_t index, std::unique_lock<SpinLock> lock) {
        ReadLock readLock (mtx);
        size_t l = LeftChild(index);
        size_t r = RightChild(index);
        size_t largest;
        if (array.size() < l) {
            lock.unlock();
            return;
        }
        if (array.size() > l) {
            auto leftLock = GetLock(l, true);
            auto rightLock = GetLock(r, true);
            std::lock(leftLock, rightLock);
            if (comp(array[l].first, array[index].first))
                largest = l;
            else 
                largest = index;
            if (comp(array[r].first, array[largest].first))
                largest = r;
            if (largest != index) {
                std::swap(array[index].first, array[largest].first);
                lock.unlock(); // unlock all except which will shifted fown in next iteration
                if (largest == l) { 
                    rightLock.unlock();
                    ShiftDown(l, std::move(leftLock));
                } else {
                    leftLock.unlock();
                    ShiftDown(r, std::move(rightLock));
                }
            }
        } else { 
            auto leftLock = GetLock(l);
            if (comp(array[index].first, array[l].first)) {
                std::swap(array[index].first, array[l].first);
                lock.unlock();
                ShiftDown(l, std::move(leftLock));
            } else {
                lock.unlock();
                leftLock.unlock();
            }
        }
    }
    
};
























    
