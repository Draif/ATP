#pragma once
#include <boost/thread.hpp>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <random>
#include <future>
#include <vector>
#include <iostream>

std::vector<int> GenerateRandomArray(size_t size) {
    std::vector<int> returnArr (size);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed); 
    //std::default_random_engine generator;
    //std::uniform_int_distribution<int> distribution(0,10);
    for (size_t i = 0; i < size; ++i)
        returnArr[i] = generator();
    return returnArr;
}

template <typename RAI>
void DebugPrint(RAI from, RAI to) {
    for(; from != to; ++from)
        std::cerr << *from << " ";
    std::cerr << std::endl;
}

template <typename RandomAccessIterator>
void ParallelQuickSort(RandomAccessIterator first, RandomAccessIterator last) {
    if (std::distance(first, last) == 0) 
        return;
    typedef typename std::iterator_traits<RandomAccessIterator>::reference TRef;
    auto const& pivot = *first;
    RandomAccessIterator divide_point = std::partition(first, last, 
        [&](const TRef t) { return t < pivot;});

    auto lower (std::async(ParallelQuickSort<RandomAccessIterator>, first, divide_point));
    if (*divide_point == pivot)
        ParallelQuickSort(std::next(divide_point), last);
    else 
        ParallelQuickSort(divide_point, last);
    lower.get();
    return;
}
