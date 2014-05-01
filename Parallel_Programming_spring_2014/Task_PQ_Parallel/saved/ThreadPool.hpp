#include "Task_PQ.hpp" 
#include <vector>
#include <thread>
#include <atomic>
#include <future>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>

class JoinThreads {
public:
    explicit JoinThreads(std::vector<boost::thread>& thr) :
        threads(thr) {}
        
    ~JoinThreads() {
        for (unsigned int i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
private:
    std::vector<boost::thread>& threads;
};

template <class TTask>
class ThreadPoolTask {
    mutable boost::promise<typename std::result_of<TTask()>::type> prom;
    TTask task;
    long long priority;
public:
    ThreadPoolTask() {}
    ThreadPoolTask(const ThreadPoolTask& another) {
        task = another.task;
        priority = another.priority;
        prom.swap(another.prom);
    }

    ThreadPoolTask& operator=(const ThreadPoolTask& another) {
        task = another.task;
        priority = another.priority;
        prom.swap(another.prom);
        return *this;
    } 

    ThreadPoolTask(const TTask& _task, long long pri):
        task(_task), priority(pri) {}
     
    void operator()() {
        prom.set_value(task());
    }
    
    bool operator>(const ThreadPoolTask& another) const {
        return priority > another.priority;
    }

    std::future<typename std::result_of<TTask()>::type> Get_Future() {
        return std::move(prom.get_future());
    }

    void Swap(ThreadPoolTask& another) {
        std::swap(task, another.task);
        std::swap(priority, another.priority);
        prom.swap(another.prom);
    }
    
};

template <class TTask>
class Task {
public:
    Task() {}  
    explicit Task(const TTask& _task, long long prio) {
        ptr = std::make_shared<ThreadPoolTask<TTask>>(_task, prio);    
    }
    Task(const Task& another) {
        ptr.reset();
        ptr = another.ptr;
    }
    Task& operator=(const Task& another) {
        ptr.reset();
        ptr = another.ptr;
        return *this;
    }
    
    ~Task() {
        ptr.reset();
    }
    void operator()() {
        ptr->operator()();
    }
    bool operator>(const Task& another) const {
        return *ptr > *another.ptr;
    }

    void Swap(Task& another) {
        ptr.swap(another.ptr);
    }
private:
    std::shared_ptr<ThreadPoolTask<TTask>> ptr;
};

template <class TTask>
class ThreadPool {
public:
    ThreadPool() : queue(), done(false), joiner(threads) {
        int hard_conc = std::thread::hardware_concurrency();
        unsigned int thread_count = (hard_conc > 0)? hard_conc:2;
        //unsigned int thread_count = 1;
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.push_back((boost::thread(&ThreadPool::worker_thread, this)));
            }
        } catch(...) {
            std::cerr << "Something was broken down!\n";
            done = true;
        }
    }
    ~ThreadPool() {
        while(!queue.empty()) {
            std::this_thread::yield();
        }
        done = true;
        queue.finishWork();
        joiner.~JoinThreads();
    }

    std::future<typename std::result_of<TTask()>::type> submit(const TTask& task, long long priority) {
        Task<TTask> node (task, priority);
        std::future<typename std::result_of<TTask()>::type> fut; //= node.Get_Future();
        queue.put(node);
        //std::cerr <<"Submit\n";
        return std::move(fut);
    }

private:
    std::atomic_bool done;
    std::vector<boost::thread> threads;
    ConcurrentPQ<Task<TTask>> queue;
    JoinThreads joiner;
private:
    void worker_thread() {
       Task<TTask> temp;
        while (!done) {
            if (!queue.pop(temp)) 
                break;
            temp();
            //std::cerr << "task has been done\n";    
        }
    }
};
