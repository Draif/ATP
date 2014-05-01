#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include "ThreadSafeLog.hpp"
#include "ThreadPool.hpp"
#include <iostream>
#include <string>

TLog<100> logging (std::cout);
FILE* testFile = fopen("test.txt", "w");

class MyTest {
    std::string message;
public:
    MyTest() {}
    MyTest(std::string mes): message(mes) {}
    MyTest(const MyTest& another) {
        message = another.message;
    }
    MyTest& operator=(const MyTest& another) {
        message = another.message;
        return *this;
    }
    int operator()() {
        //logging << message;
        message.push_back('\n');
        fputs(message.c_str(), testFile);
        return 1;
    }
};

const int num = 10000;

BOOST_AUTO_TEST_CASE(my_test) {
    {ThreadPool<MyTest> pool;
    for (int i = 0; i < num; ++i) {
        pool.submit(MyTest("Thread logging " + std::to_string(i)), num - i);
    }
    logging << "The end! Everything is ok!\n";}
    fclose(testFile);
}

    
