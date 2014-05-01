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

template <class Node, class Compare = MyComp<Node>>
class ConcurrentPQ {
    class CellLockGuard {
    public:
        CellLockGuard() {
            cells.push_back(boost::detail::spinlock());
        }
        void lock(size_t i) {
            cells[i].lock();   
        }
        void unlock(size_t i) {
            cells[i].unlock();
        }
        void push_back() {
            cells.push_back(boost::detail::spinlock());
            cells.back().unlock();
        }
        void pop_back() {
            cells.pop_back();
        }
    private:
        std::vector<boost::detail::spinlock> cells;
    };

public:
    ConcurrentPQ(): done(false) {
        array.push_back(Node());
    }

    void finishWork() {
        done = true;
        cv.notify_all();
    }
    void put(const Node& task) {
        //std::cerr << "put begin\n";
        mtx.lock();
        array.insert(array.end(), task);
        lockGuard.push_back();
        ShiftUp(array.size() - 1);
        cv.notify_one();
        mtx.unlock();
        //std::cerr << "put end\n";
    }

     bool pop(Node& node) {
        //std::cerr << boost::this_thread::get_id() << " try_pop begin\n";
        {
            boost::unique_lock<boost::mutex> lock (pop_mtx);
            while (!(array.size() > 1) && (!done)) cv.wait(lock);
            if (done) 
                return false;
            //std::cerr << "end of wait\n";
            lockGuard.lock(1);
            //std::cerr << "lockGuard locked\n";
            mtx.lock();
            //std::cerr << "mutex locked\n";
            node  = array[1];
            //std::cerr << "moved\n";
            if (array.size() > 2) {
                //std::cerr << "1\n";
                array[1] = array.back();
                //std::cerr << "2\n";
            }
            array.pop_back();
            lockGuard.pop_back();
            //std::cerr << "4\n";
            mtx.unlock();
            //std::cerr << "mutex unlocked\n";
        }
        if (array.size() > 1) {
            mtx.lock_shared();
            //std::cerr << "lock_shared\n";
            ShiftDown(1);
            //std::cerr << "Shift down\n";
            mtx.unlock_shared();
        } 
        //std::cerr << boost::this_thread::get_id() << " try pop return true\n";
        return true;
    }

    bool empty() {
        mtx.lock_shared();
        if (array.size() == 1) {
            mtx.unlock_shared();
            return true;
        }
        mtx.unlock_shared();
        return false;
    }
    
private:
    boost::atomic_bool done;
    boost::shared_mutex mtx;
    boost::mutex locking;
    boost::mutex pop_mtx;
    std::vector<Node> array;
    CellLockGuard lockGuard;
    Compare comp;
    boost::condition_variable cv;
    size_t Parent(size_t i) {
        return i/2;
    }
    size_t LeftChild(size_t i) {
        return 2*i;
    }
    size_t RightChild(size_t i) {
        return 2*i +1;
    }
    void Swap(size_t a, size_t b) {
        Node temp = std::move(array[a]);
        array[a] = std::move(array[b]);
        array[b] = std::move(temp);
    }
    void ShiftUp(size_t i) {
        while(i > 1 &&  comp(array[i], array[Parent(i)])) {
            array[i].Swap(array[Parent(i)]);
            i = Parent(i);
        }
    }
    void ShiftDown(size_t i) {
        size_t l = LeftChild(i);
        size_t r = RightChild(i);
        size_t largest;
        { // lock all three nodes
            boost::lock_guard<boost::mutex> lock(locking);
            if (l < array.size())
                lockGuard.lock(l);
            if (r < array.size())
                lockGuard.lock(r);
        }
        //std::cerr << "locked all needed\n";
        if (l < array.size() && comp(array[l], array[i]))
            largest = l;
        else 
            largest = i;
        if (r < array.size() && comp(array[r], array[largest]))
            largest = r;
        if (largest != i) {
            //std::cerr << "Swaping begin\n";
            array[i].Swap(array[largest]);
            lockGuard.unlock(i); // unlock all except which will shifted fown in next iteration
            if (largest == l && r < array.size()) 
                lockGuard.unlock(r); 
            else if (l < array.size())
                lockGuard.unlock(l);
            //std::cerr << "Unlock and going next\n";
            ShiftDown(largest);
        } else { 
        // unlock all 
            if (l < array.size())
                lockGuard.unlock(l); 
            if (r < array.size())
                lockGuard.unlock(r); 
            lockGuard.unlock(i); 
            //std::cerr << "Unlock all\n";
        }
    }
    
};























    
