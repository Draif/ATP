#pragma once
#include "Task_PQ.hpp" 
#include <vector>
#include <thread>
#include <atomic>
#include <future>
#include <iostream>
#include <condition_variable>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>

class JoinThreads {
public:
    explicit JoinThreads(std::vector<boost::thread>& thr) :
        threads(thr) {}
        
    ~JoinThreads() {
        for (unsigned int i = 0; i < threads.size(); ++i) {
                threads[i].join();
        }
    }
private:
    std::vector<boost::thread>& threads;
};

template <class Result>
class Data {
public:
    Data(): ready(false) {}
    
    void set(Result res) {
        result = res;
        ready = true;
        cv.notify_all();
    }

    Result get (){
        std::unique_lock<std::mutex> lock (mtx);
        cv.wait(lock, [this]()->bool{return ready;});
        return result;
    }

private:
    bool ready;
    std::mutex mtx;
    Result result;
    std::condition_variable cv;
};

template <class Result>
class Future {
public:
    Future(): result(new Data<Result>()) {
    }
    Future(const Future& another) {
        result.reset();
        result = another.result;
    }
    Future& operator=(const Future& another) {
        result.reset();
        result = another.result;
        return *this;
    }

    void set(Result res) {
        result->set(res);
    }

    Result get (){
        return result->get();
    }

private:
    std::shared_ptr<Data<Result>> result;
};

template <>
class Future<void> {
public:
    Future() {}
    Future(const Future& another) {
    }
    Future& operator=(const Future& another) {
        return *this;
    }

    void set() {
    }

    void get (){
    }
};



template <class TTask>
class ThreadPoolTask {
    TTask task;
    long long priority;
    Future<typename std::result_of<TTask()>::type> fut;
public:
    ThreadPoolTask(const TTask& _task, long long pri):
        task(_task), priority(pri){}
    ThreadPoolTask(const TTask& _task) : task(_task) {} 
    ThreadPoolTask(const ThreadPoolTask& another) {
        task = another.task;
        priority = another.priority;
        fut = another.fut;
    }

    ThreadPoolTask& operator=(const ThreadPoolTask& another) {
        task = another.task;
        priority = another.priority;
        fut = another.fut;
        return *this;
    } 
     
    void operator()() {
        fut.set(task());
    }
    
    bool operator>(const ThreadPoolTask& another) const {
        return priority > another.priority;
    }

    Future<std::result_of<TTask()>> GetFuture() {
        return fut;
    }

};


template <  class ReturnType, 
            typename Heap = ConcurrentPQ<ThreadPoolTask<std::function<ReturnType(void)>>>
            >
class ThreadPool {
    typedef std::function<ReturnType(void)> TTask;
public:
    ThreadPool(int numberOfThreads) : queue(), done(false), joiner(threads) {
        int hard_conc = (numberOfThreads)? numberOfThreads : std::thread::hardware_concurrency();
        unsigned int thread_count  = (hard_conc > 0)? hard_conc : 2;
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
        ThreadPoolTask<TTask> node (task, priority);
        std::future<typename std::result_of<TTask()>::type> fut; //= node.Get_Future();
        queue.put(node);
        return std::move(fut);
    }

    std::future<typename std::result_of<TTask()>::type> submit(const TTask& task) {
        ThreadPoolTask<TTask> node (task);
        std::future<typename std::result_of<TTask()>::type> fut; //= node.Get_Future();
        queue.put(node);
        return std::move(fut);
    }

private:
    std::atomic_bool done;
    std::vector<boost::thread> threads;
    //Heap<ThreadPoolTask<TTask>> queue;
    Heap queue;
    JoinThreads joiner;
private:
    void worker_thread() {
       ThreadPoolTask<TTask> temp(TTask(), 0);
        while (!done) {
            if (queue.pop(temp)) 
                temp();
            else 
                boost::this_thread::yield();
        }
    }
};
