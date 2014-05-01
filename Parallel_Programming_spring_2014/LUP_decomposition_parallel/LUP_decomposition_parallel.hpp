/*
 * =====================================================================================
 *
 *       Filename:  LUP_decomposition_parallel.hpp
 *
 *    Description:  Parallel version of LUP decomposition. More information of LUP at
 *                  http://en.wikipedia.org/wiki/LUP_decomposition
 *
 *        Version:  1.0
 *        Created:  01.05.2014 12:11:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <exception>
#include <vector>
#include <atomic>
#include "Matrix.hpp"
#include "ThreadPool.hpp"
#include "Task_PQ.hpp"

namespace Draif {

    //class for result of LUP decomposition. Store Matrixes P, L and U
    template <class T>
    class LUP_Matrixes {
    public:
        LUP_Matrixes() {}
        LUP_Matrixes(const Matrix<T>& _L, const Matrix<T>& _U, const Matrix<T>& _P) : L(_L), U(_U), P(_P) {}
        
        LUP_Matrixes(const LUP_Matrixes& another) {
            L = another.L;
            U = another.U;
            P = another.P;
        }
        
        LUP_Matrixes& operator=(const LUP_Matrixes& another) {
            if (this != &another) { 
                L = another.L;
                U = another.U;
                P = another.P;
            }
            return *this;
        }
        
        const Matrix<T>& GetU() const {
            return U;
        }
        
        const Matrix<T>& GetL() const {
            return L;
        }
        
        const Matrix<T>& GetP() const {
            return P;
        }
    private:
        Matrix<T> L;
        Matrix<T> U;
        Matrix<T> P;
    };

    //simple task, whick is pushed in ThreadPool
    template <class T>
    class LUPTask {
        typedef std::vector<T>& ref;
        size_t from;
        ref A;
        ref U;
        T l;
        std::atomic<int>& sem;
    public:
        LUPTask(size_t _from, ref _A, ref _U, T _l, std::atomic<int>& _sem) : from(_from), A(_A), U(_U), l(_l), sem(_sem) {}

        int operator()() {
            for (size_t i = from; i < A.size(); ++i) 
                A[i] -= l * U[i];
            ++sem;
            return 1;
        }
    };


    //T should be real number
    //main function itself
    template <class T>
    LUP_Matrixes<T> LUPDecompositionParallel(Matrix<T> A) {
        size_t size = A.GetRowsNumber();
        Matrix<T> L(size, size, 0);
        Matrix<T> U(size, size, 0);
        Matrix<T> P(size, size, 0);
        std::atomic<int> semaphore(0);
        std::vector<size_t> permutations;
        ThreadPool<int, ConcurentQueue<ThreadPoolTask<std::function<int(void)>>>> pool;

        //originally, chose A[k][k] as pivot
        for (size_t i = 0; i < size; ++i)
            permutations.push_back(i);
        
        //main loop
        for (size_t k = 0; k < size; ++k) { 
            double p = 0.0;
            size_t swapRow = k;
            
            //find pivot for next iteration
            for (size_t i = k; i < size; ++i)
                if (fabs(A[i][k]) > p) {
                    p = fabs(A[i][k]); //get the largest pivot to decrease calculation accuracy
                    swapRow = i;
                }
            
            //if we can't find pivot != 0
            if (p == 0.0)
                throw std::runtime_error("Matrix is degenerated!");
            
            //swap rows for new pivot
            std::swap(permutations[k], permutations[swapRow]);
            A.SwapRows(k, swapRow);
            
            //the main calculation. Push tasks in pool
            for (size_t i = k + 1; i < size; ++i) {
                A[i][k] /= A[k][k];
                pool.submit(LUPTask<T>(k + 1, A[i], A[k], A[i][k], semaphore));
            }
            
            //wait, while all tasks will be done
            while (semaphore.load() != size - k - 1);
            semaphore.store(0);
        }
        
        //calculation of matrixes L, U, P
        for (size_t i = 0; i < size; ++i) { 
            P[i][permutations[i]] = 1;
            for (size_t j = i; j < size; ++j){
                if (i == j) 
                    L[i][j] = 1;
                else 
                    L[size - i - 1][size - j - 1] = A[size - i - 1][size - j - 1];
                U[i][j] = A[i][j];
            }
        }

        return LUP_Matrixes<T>(L, U, P);
    }
}
