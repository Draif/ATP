#include <iostream>
#include "ThreadPool.hpp"

class MyTask {
    int a;
public:
    MyTask() {}
    MyTask(int _a): a(_a) {
        //std::cerr << "a equals to " << a << std::endl;
    }
    MyTask(const MyTask& another) {
        a = another.a;
    }
    MyTask& operator=(const MyTask& another) {
        a = another.a;
        return *this;
    }
    int operator()() {
        std::cout << "Task print: " << a << std::endl;
        return 1;
    }
};

class SampleTask {
    int& value;
    int key;
public:
    SampleTask(int k, int& v)
        : value(v) 
        , key(k)
    {}

    int operator()() {
        value = 0;
        for(size_t i = 0; i < 10; ++i)
            value += i * i * key;
        return value;
    }
};  


int main() {
    try{
        ThreadPool<int> tp;
        std::vector<int> v (5000);
        for (int i = 0; i < 5000; ++i) 
            tp.submit(MyTask(i+1), i+1);
        for (int i = 0; i < 5000; ++i)
            tp.submit(SampleTask(i, v[i]), i+1);
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "all correct!\n";
    return 0;
}
