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
#include <cmath>
#include <iostream>
#include <chrono>
#include <random>
#include <exception>

//usual test
void Test(size_t size) {
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


//test solving SLE via LUP decomposition
void Test2(size_t size) {
    Matrix<double> A(size, size);
    std::vector<double> X (size); 
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed); 
    
    //generate random matrix and random unknown values
    for (size_t i = 0; i < size; ++i) {
        X[i] = generator() % 10000;
        for (size_t j = 0; j < size; ++j)
            A[i][j] = generator() % 10000;
    }
    //calculate the result of A * X = B
    std::vector<double> result(size);
    for (int i = 0; i < size; ++i) {
        double temp = 0.0;
        for (int j = 0; j < size; ++j)
            temp += X[j] * A[i][j];
        result[i] = temp;
    }

    //process LUP decomposition
    Draif::LUP_Matrixes<double> res;
    try {
        res = Draif::LUPDecompositionParallel(A);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    //usual check for right LUP decomposition
    BOOST_CHECK_EQUAL(res.GetP() * A, res.GetL() * res.GetU());
    //temp vectors
    std::vector<double> initialX (size);
    std::vector<double> y (size);
    //make vector of permutation from P matrix, which is result of LUP 
    std::vector<int> P (size);
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (res.GetP()[i][j])
                P[i] = j;
    
    //solving SLE - more information  - Kormen 2nd edition p844
    for (int i = 0; i < size; ++i) {
        double temp = 0.0;
        for (int j = 0; j <= i -1; ++j)
            temp += res.GetL()[i][j] * y[j];
        y[i] = result[P[i]] - temp;
    }

    for(int i = size - 1; i >= 0; --i) {
        double temp = 0.0;
        for (int j = i + 1; j < size; ++j)
            temp += res.GetU()[i][j] * initialX[j];
        const double eps = 0.001;
        if (fabs(res.GetU()[i][i]) >= eps)
           initialX[i] = (y[i] - temp) / res.GetU()[i][i];
        else 
           initialX[i] = 0;
    }

    //check we had right solving of LSE. Compare to initial X
    auto oper = [](double a, double b) { 
        const double eps = 0.001; 
        return (a - eps <= b) && (a + eps >= b);
    };
    for (int i = 0; i < size; ++i)
        BOOST_CHECK(oper(X[i], initialX[i]));
}

BOOST_AUTO_TEST_CASE(Main) {
    size_t numberOfTests = 50;
    for (size_t i = 0; i < numberOfTests; ++i) {
        std::cout << "Test number: " << i << std::endl;
        Test(100);
        Test2(100);
    }
}















