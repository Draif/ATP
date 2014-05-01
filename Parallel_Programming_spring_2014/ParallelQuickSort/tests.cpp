#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include "ParallelQuickSort.hpp"
#include <vector>
#include <iostream>

bool checkArray(std::vector<int>& arr) {
    for (int i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) 
            return false;
    }
    return true;
}

void PrintArray(std::vector<int>& arr) {
    std::cerr << "Printing array\n";
    for (auto ptr = arr.begin(); ptr != arr.end(); ++ptr)
        std::cerr << *ptr << " ";
    std::cerr << std::endl;
}


BOOST_AUTO_TEST_CASE(sort_test) { 
    int size = 1;
    for (int i = 0; i < 5; ++i) {
        size *= 10;
        std::vector<int> arr = GenerateRandomArray(size);
        ParallelQuickSort(arr.begin(), arr.end());
        BOOST_CHECK(checkArray(arr));
    }
}
