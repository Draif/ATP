/*
 * =====================================================================================
 *
 *       Filename:  Polynomial.hpp
 *
 *    Description:  Multiplication of two polynomials for the O(n * log n) using 
 *                  parallel FFT. More information about it see on Korman (2nd edition, p938).
 *
 *        Version:  1.0
 *        Created:  02.05.2014 16:24:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include "FFT_Parallel.hpp"
#include <vector>

namespace Draif {
    
    
    class Polynomial {
    public:
        Polynomial() {}
        
        Polynomial(const std::vector<std::complex<double>> _pol) : pol(_pol) {}
        
        Polynomial(const Polynomial& another) {
            pol = another.pol;
        }

        Polynomial& operator=(const Polynomial& another) {
            if (this != &another) {
                pol = another.pol;
            }
            return *this;
        }

        const std::complex<double>& GetCoefficent(size_t i) const {
            if (i >= pol.size())
                throw std::runtime_error("Polynomial::GetCoefficent - ouf of range");
            return pol[i];
        }
        
        void ChangeCoefficent(size_t degree, std::complex<double> val) {
            if (degree >=  pol.size())
                pol.resize(degree + 1);
            pol[degree] = val;
        }

        size_t Degree() const {
            return pol.size() - 1;
        }

        size_t Size() const {
            return pol.size();
        }

        void Resize(size_t newSize, std::complex<double> val = std::complex<double>(0,0)) {
            pol.resize(newSize, val);
        }

        std::vector<std::complex<double>>& Coefficents() {
            return pol;
        }

        void ChangeAllCoefficents(const std::vector<std::complex<double>>& coef) {
            pol = coef;
        }
        
        void RemoveZeroDegrees() {
            const double eps = 0.1;
            while (pol.back().real() < eps && pol.back().real() > -eps)
                pol.pop_back();
        }

    private:
        std::vector<std::complex<double>> pol;
    };
    
       
    void IncreaseSizePow2(Polynomial& pol) {
        size_t size = static_cast<size_t>(pow(2.0, ceil(log2(static_cast<double>(pol.Size())))));
        pol.Resize(size, 0);
    }

    Polynomial PolynomialMultiplication(Polynomial first, Polynomial second) {
        size_t size = (first.Size() > second.Size()) ? first.Size() : second.Size();
        
        first.Resize(2 * size); 
        second.Resize(2 * size); 
        
        IncreaseSizePow2(first);
        IncreaseSizePow2(second); 
        //FFT of first and second
        first.ChangeAllCoefficents(Draif::FFTParallel(first.Coefficents()));
        second.ChangeAllCoefficents(Draif::FFTParallel(second.Coefficents()));
        //multiplying of coefficents
        for (int i = 0; i < first.Size(); ++i)
            first.ChangeCoefficent(i, first.GetCoefficent(i) * second.GetCoefficent(i));
        //reverse FFT coefficents
        first.Coefficents() = Draif::FFTParallel(first.Coefficents(), true);
        // remove zero degrees 
        first.RemoveZeroDegrees();
        return first;
    }


}
