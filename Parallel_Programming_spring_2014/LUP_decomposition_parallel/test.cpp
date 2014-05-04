/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  Test for LUP_decomposition_parallel.hpp
 *
 *        Version:  1.0
 *        Created:  01.05.2014 10:08:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), iakovenkopavel@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include "LUP_decomposition_parallel.hpp"
#include "Matrix.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <exception>

//usual test
void Test() {
    const size_t size = 100;
    Matrix<double> A(size, size);
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed); 
    
    //generate random matrix
    for (size_t i = 0; i < size; ++i)
        for (size_t j = 0; j < size; ++j)
            A[i][j] = generator() % 10000;
    
    //process LUP decomposition
    Draif::LUP_Matrixes<double> res;
    try {
        res = Draif::LUPDecompositionParallel(A);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    
    //check whethter P*A == L*U is true
    BOOST_CHECK_EQUAL(res.GetP() * A, res.GetL() * res.GetU());
}

BOOST_AUTO_TEST_CASE(Main) {
    size_t numberOfTests = 50;
    for (size_t i = 0; i < numberOfTests; ++i)
        Test();
}
