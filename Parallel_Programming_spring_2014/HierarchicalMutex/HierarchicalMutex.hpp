#pragma once
#include <set>
#include <exception>
#include <boost/thread.hpp>
#include <mutex>


class HMException:public std::exception {
    std::string error;
public:
    HMException(const char* err): error(err) {}
    const char* what() {
        return error.c_str();
    }
};


class HierarchicalMutex {
public:
    HierarchicalMutex(size_t _priority): Priority(_priority) {}

    void lock() {
        checkPossiblePriority();
        InternalMutex.lock();
        LastPriority.insert(Priority);
    }

    bool try_lock() {
        checkPossiblePriority();
        if (!InternalMutex.try_lock())
            return false;
        LastPriority.insert(Priority);
        return true;
    }

    void unlock() {
        LastPriority.erase(Priority);
        InternalMutex.unlock();
    }

private:
    std::mutex InternalMutex;
    size_t Priority;
    static thread_local std::set<size_t> LastPriority;
    static thread_local std::set<size_t> UsedPriorities;
    
    bool checkPossiblePriority() {
        if (LastPriority.empty()) {
            return true;
        }
        if (LastPriority.find(Priority) != LastPriority.end()) 
            throw HMException("already locked!");
        if (Priority > *(LastPriority.rbegin())) {
            throw HMException("wrong priority!");
        }
        return true;
    }
};

thread_local std::set<size_t> HierarchicalMutex::LastPriority;
