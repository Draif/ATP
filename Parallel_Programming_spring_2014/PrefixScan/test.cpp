/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  Test for PrefixScan.hpp
 *
 *        Version:  1.0
 *        Created:  21.04.2014 23:08:53
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
#include "PrefixScan.hpp"
#include <iostream>
#include <vector>

class Number {
public:
    Number() : number(0) {}
    Number(unsigned long long _number) : number(_number) {}
    Number(const Number& another) {
        number = another.number;
    }
    unsigned long long GetNumber() const {
        return number;
    }
    void SetNumber(int n) {
        number = n;
    }
    Number& operator=(const Number& another) {
        number = another.number;
    }

    bool operator==(const Number& another) const {
        return number == another.number;
    }
private:
    unsigned long long number;
};

std::ostream& operator<<(std::ostream& o, const Number& num) {
    o << num.GetNumber();
    return o;
}

class Sum {
public:
    Number operator()(Number& a, Number& b) {
         return Number(a.GetNumber() + b.GetNumber());
    }
};

class Permutation {
public:
    Permutation() {
        for (int i = 1; i <= 10; ++i) 
            permutation.push_back(i);
    }
    Permutation(std::vector<int> perm) : permutation(perm) {}
    const std::vector<int>& GetPerm() {
        return permutation;
    }
    bool operator==(const Permutation& another) {
        for (int i = 0; i < permutation.size(); ++i)
            if (permutation[i] != another.permutation[i])
                return false;
        return true;
    }
private:
    std::vector<int> permutation;
};

class PermMult {
public:
    Permutation operator()(Permutation& a, Permutation& b) {
        size_t size = a.GetPerm().size();
        std::vector<int> res (size);
        for (int i = 0; i < size; ++i) 
            res[i] = b.GetPerm()[a.GetPerm()[i] - 1];
        return Permutation(res);
    }
};

//test for prefix sum
BOOST_AUTO_TEST_CASE(first_test) {
    const long long arraySize = 10000;
    std::vector<Number> arr;
    for (int i = 0; i < arraySize; ++i) 
        arr.push_back(Number(i + 1));
    unsigned long long  prefixSum =  Draif::PrefixScan<Number, Sum>(arr).GetNumber();
    unsigned long long sum = (arraySize + 1) * arraySize / 2;
    BOOST_CHECK_EQUAL(prefixSum, sum);
}

//test for multiplication of permutations
BOOST_AUTO_TEST_CASE(second_test) {
    std::vector<int> perm = {4,3,1,2,6,7,9,5,10,8}; // permutation
    std::vector<Permutation> arr (12, Permutation(perm)); // order of perm is 12
    const std::vector<int> result = Draif::PrefixScan<Permutation, PermMult>(arr).GetPerm();
    for (int i = 0; i < result.size(); ++i) {
        BOOST_CHECK_EQUAL(i + 1, result[i]);
    }
}

// perfect test - check all elements
BOOST_AUTO_TEST_CASE(third_test) {
    const long long arraySize = 10000;
    std::vector<Number> arr;
    std::vector<Number> resultArray;
    for (int i = 0; i < arraySize; ++i) { 
        arr.push_back(Number(i + 1));
        resultArray.push_back(Number(i + 1));
    }
    Draif::PrefixScan<Number, Sum>(arr);
    Sum sum;
    for (int i = 1; i < resultArray.size(); ++i) 
        resultArray[i] = sum(resultArray[i - 1], resultArray[i]);
    for (int i = 0; i < arr.size(); ++i) 
        BOOST_CHECK_EQUAL(arr[i].GetNumber(), resultArray[i].GetNumber());
}


//test for multiplication of permutations - check all elements
BOOST_AUTO_TEST_CASE(fourth_test) {
    std::vector<int> perm = {4,3,1,2,6,7,9,5,10,8}; // permutation
    std::vector<Permutation> arr (12, Permutation(perm)); // order of perm is 12
    std::vector<Permutation> resArr (12, Permutation(perm)); // order of perm is 12
    Draif::PrefixScan<Permutation, PermMult>(arr);
    PermMult mul;
    for (int i = 1; i < resArr.size(); ++i) 
        resArr[i] = mul(resArr[i - 1], resArr[i]);
    for (int i = 0; i < perm.size(); ++i) 
        BOOST_CHECK(arr[i] == resArr[i]);
}
