/*
 * =====================================================================================
 *
 *       Filename:  tests.cpp
 *
 *    Description:  Tests for HierarchicalMutex.hpp
 *
 *        Version:  1.0
 *        Created:  12.04.2014 19:32:30
 *       Revision:  none
 *       Compiler:  clang++ 
 *
 *         Author:  Pavel Iakovenko (Draif), iakovenkopavel@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <atomic>
#include <functional>
#include "HierarchicalMutex.hpp"
// usual test for working
BOOST_AUTO_TEST_CASE(my_test1) {
    HierarchicalMutex mtx1(2);
    HierarchicalMutex mtx2(1);
    boost::unique_lock<HierarchicalMutex> lk1(mtx1);
    boost::unique_lock<HierarchicalMutex> lk2(mtx2);
}
// try lock already locked mutex
BOOST_AUTO_TEST_CASE(my_test11) {
    try {
        HierarchicalMutex mtx1(2);
        HierarchicalMutex mtx2(1);
        boost::unique_lock<HierarchicalMutex> lk1(mtx1);
        boost::unique_lock<HierarchicalMutex> lk2(mtx1);
    } catch (HMException e) {
        std::cerr << e.what() << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(my_test2) {
    try {
        HierarchicalMutex mtx1(1);
        HierarchicalMutex mtx2(2);
        mtx1.lock();
        mtx2.lock();
        mtx2.unlock();
        mtx1.unlock();
    } catch (HMException e) {
        std::cerr << "HMException was caught!\n";
        std::cerr << e.what() << std::endl;
    }
}

void function() {
    HierarchicalMutex mtx1(2);
    HierarchicalMutex mtx2(1);
    mtx1.lock();
    mtx2.lock();
    mtx2.unlock();
    mtx1.unlock();
    try {
        mtx2.lock();
        mtx1.lock();
    } catch (HMException e) {
    }
}


BOOST_AUTO_TEST_CASE(my_test3) {
    for (int i = 0; i < 5; ++i) 
        boost::thread(function);
}

void eat(HierarchicalMutex& mtx1, HierarchicalMutex& mtx2, int& eaten, const int& limit, std::atomic<bool>& start) {
    while (!start.load());
    while (eaten < 10) {
        try { 
            mtx1.lock();
            if (mtx2.try_lock()) {
                ++eaten;
                mtx2.unlock();
            }
            mtx1.unlock();
        } catch (HMException e) {
            mtx2.lock();
            if (mtx1.try_lock()) {
                ++eaten;
                mtx1.unlock();
            }
            mtx2.unlock();
        }
    }
}

BOOST_AUTO_TEST_CASE(philosophers) {
    //spoons
    HierarchicalMutex spoon1(1);
    HierarchicalMutex spoon2(2);
    HierarchicalMutex spoon3(3);
    std::atomic<bool> start(false);
    //what number each philosofer should eat
    const int limit = 10;
    //number of eaten spagetties
    int eaten1 = 0;
    int eaten2 = 0;
    int eaten3 = 0;
    boost::thread first(eat, std::ref(spoon2), std::ref(spoon1), std::ref(eaten1), std::ref(limit), std::ref(start));
    boost::thread second(eat, std::ref(spoon3), std::ref(spoon2), std::ref(eaten2), std::ref(limit), std::ref(start));
    boost::thread third(eat, std::ref(spoon3), std::ref(spoon1), std::ref(eaten3), std::ref(limit), std::ref(start));
    start.store(true);
    first.join();
    second.join();
    third.join();
    BOOST_CHECK_EQUAL(eaten1, limit);
    BOOST_CHECK_EQUAL(eaten2, limit);
    BOOST_CHECK_EQUAL(eaten3, limit);
}




