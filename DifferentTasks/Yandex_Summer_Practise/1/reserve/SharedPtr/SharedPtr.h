/*
 * =====================================================================================
 *
 *       Filename:  SharedPtr.h
 *
 *    Description:  analog of std::shared_ptr<>
 *
 *        Version:  1.0
 *        Created:  27.04.2014 17:26:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <map>
template <class T>
class DefaultDeleter {
public:
    void operator()(T* ptr) {
        delete ptr;
    }
};

template <class T, class Deleter = DefaultDeleter<T> >
class SharedPtr {
public:
    SharedPtr() : ptr(0) {}
    
    SharedPtr(T* _ptr) : ptr(_ptr) {
        if (countNumber.count(ptr))
            ++countNumber[ptr];
        else
            countNumber[ptr] = 1;
    }

    SharedPtr(const SharedPtr& another) {
        ptr = another.ptr;
        ++countNumber[ptr];
    }

    ~SharedPtr() {
        if (ptr != 0)
            deletePtr();
    }

    SharedPtr& operator=(const SharedPtr& another) {
        if (this != &another) {
            if (ptr != 0) 
                deletePtr();
            ptr = another.ptr;
            ++countNumber[ptr];
        }
        return *this;
    }

    operator bool() const { 
        return ptr != 0;
    }

    T* Get() {
        return ptr;
    }
    
    const T* Get() const {
        return ptr;
    }

    void reset() {
        deletePtr();
    }

    T& operator*() {
        return *ptr;
    }
    
    const T& operator*() const {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    const T* operator->() const {
        return ptr;
    }

    bool Unique() const {
        if (ptr != 0)
            return countNumber[ptr] == 1;
        return false;
    }

    long int UseCount() const {
        if (ptr != 0)
            return countNumber[ptr];
        return 0;
    }

    void Swap(SharedPtr& another) {
        std::swap(ptr, another.ptr);
    }

private:
    
    void deletePtr() {
        if (countNumber[ptr] == 1) {
            countNumber.erase(ptr);
             del(ptr);
        } else 
            --countNumber[ptr];
    }
    
    T* ptr;
    Deleter del;
    static std::map<T*, long int> countNumber;
};

template <class T, class Deleter>
std::map<T*, long int> SharedPtr<T, Deleter>:: countNumber;
