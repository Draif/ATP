/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  Test for PolynominalMultiplication.hpp
 *
 *        Version:  1.0
 *        Created:  02.05.2014 17:07:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>
#include <boost/math/tools/polynomial.hpp>
#include "Polynomial.hpp"
#include <complex>

bool Equal(double a, double b) {
    const double eps = 0.001;
    return ((a - eps <= b) && (a + eps >= b));
}

void Test(size_t size) {
    //create random generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed); 
    
    //fulfil random vectors
    std::vector<std::complex<double>> rand1 (size);
    std::vector<std::complex<double>> rand2 (size);
    for (int i = 0; i < size; ++i) {
        rand1[i] = generator() % 10000;
        rand2[i] = generator() % 10000;
    }
    
    //create polynomials
    Draif::Polynomial first(rand1);
    Draif::Polynomial second(rand2);
    boost::math::tools::polynomial<std::complex<double>> one (&rand1.front(), size - 1);
    boost::math::tools::polynomial<std::complex<double>> two (&rand2.front(), size  - 1);
    
    //multiply two polynomials
    first = Draif::PolynomialMultiplication(first, second); 
    one *= two;
    
    //check equality of sizes and equality of coefficents
    BOOST_CHECK_EQUAL(one.size(), first.Size());
    for (int i = 0; i < one.size(); ++i) 
        BOOST_CHECK(Equal(first.GetCoefficent(i).real(), one[i].real()));
}
    
BOOST_AUTO_TEST_CASE(Main) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed); 
    // usual tests 
    Test(1);
    Test(8);
    Test(1000);
    //test with random size
    const int numberOfTests = 100;
    for (int i = 0; i < numberOfTests; ++i) {
        std::cout << "Test number: " << i + 1 << std::endl;
        Test(generator() % 1000);
    }
}
