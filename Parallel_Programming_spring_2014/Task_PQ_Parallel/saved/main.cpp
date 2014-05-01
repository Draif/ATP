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
    int operator()() const {
        std::cout << "Task print: " << a << std::endl;
        return 1;
    }
};


int main() {
    try{
        ThreadPool<MyTask> tp;
        for (int i = 0; i < 100; ++i) 
            tp.submit(MyTask(i+1), i+1);
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
