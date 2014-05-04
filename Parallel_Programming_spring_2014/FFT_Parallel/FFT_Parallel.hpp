/*
 * =====================================================================================
 *
 *       Filename:  FFT_Parallel.hpp
 *
 *    Description:  Parallel variant of Fast Fourie transforming 
 *                  (http://en.wikipedia.org/wiki/Fast_Fourier_transform)
 *                  
 *
 *        Version:  1.0
 *        Created:  01.05.2014 22:57:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
// TODO: make treeguard as lock structure

#pragma once
#include "ThreadPool.hpp"
#include "Task_PQ.hpp"
#include <vector>
#include <complex>
#include <cmath>


namespace Draif {

   //main reverse function of bits
   unsigned int reverse(unsigned int num, unsigned int lg_n) {
        int res = 0;
        for (int i=0; i<lg_n; ++i)
            if (num & (1<<i))
                res |= 1<<(lg_n-1-i);
        return res;    
    }

    //restructure elements by reversing bits of indexes 
    void BitReverseCopy(std::vector<std::complex<double>>& vec) {
        std::vector<std::complex<double>> tempVector(vec.size());
        unsigned int n = tempVector.size();
        unsigned int lg_n = 0;
	    while ((1 << lg_n) < n)  ++lg_n;
        for (unsigned int i = 0; i < vec.size(); ++i)
            tempVector[i] = vec[reverse(i, lg_n)];
        vec = tempVector;
    }
    
    //tree guard - store locks and  
    class TreeGuard {
    public:
        TreeGuard(int numberOfLists) : vec(2 * numberOfLists - 1) {
            for (int i = 0; i < vec.size(); ++i) 
                vec[i].store(false);
        }
        //check whether this node can be calculated. So, its children should have been valculated before
        bool IsReady(size_t height, size_t pos) {
            size_t position = static_cast<size_t>(pow(2.0, height)) + pos - 1;
            if (RightChild(position) < vec.size()) 
                if (vec[LeftChild(position)].load() && vec[RightChild(position)].load())
                    return true;
                else 
                    return false;
            return true;
        }
        //used, when calculation of node is over
        void MarkReady(size_t height, size_t pos) {
            size_t position = static_cast<size_t>(pow(2.0, height)) + pos - 1;
            vec[position].store(true);
        }
        //true if all tree was calculated
        bool IsTreeReady() const {
            return vec.front().load();
        }

    private:
        size_t LeftChild(size_t i) {
            return 2*i + 1;
        }
        size_t RightChild(size_t i) {
            return 2*i + 2;
        }
        std::vector<std::atomic<bool>> vec;
    };
    
    //main task used in ThreadPool
    class FFTTask {
        typedef std::vector<std::complex<double>> vec;
        vec& A;
        size_t k;
        size_t m;
        std::complex<double> step;
        TreeGuard& tree;
        size_t height;
        size_t pos;
    public:
        FFTTask(vec& _A, size_t _k, size_t _m, std::complex<double> _step, TreeGuard& t, size_t h, size_t p)
            : A(_A), k(_k), m(_m), step(_step), tree(t), height(h), pos(p) {}
        
        int operator()() {
            //wait, while we can calculate this node
            while (!tree.IsReady(height, pos)) 
                std::this_thread::yield();
            
            //main calculation
            std::complex<double> w (1.0, 0.0);
            for (size_t j = 0; j < m/2; ++j) {
                std::complex<double> t = w * A[k + j + m/2];
                std::complex<double> u = A[k + j];
                A[k + j] = u + t;
                A[k + j + m/2] = u - t;
                w *= step;
            }
            //say we end with this calculation
            tree.MarkReady(height, pos);
            return 1;
        } 
    };
    
    //get vector of coefficents of func and return FFT(func)
    std::vector<std::complex<double>> FFTParallel(std::vector<std::complex<double>> coefficents, bool invert = false) {
        BitReverseCopy(coefficents); // restructure elements 
        size_t height = static_cast<size_t> (log2(coefficents.size()));
        size_t n = coefficents.size();
        TreeGuard tree(coefficents.size() / 2);
        {
            ThreadPool<int, ConcurentQueue<ThreadPoolTask<std::function<int(void)>>>> pool;
            //more information - Kormen, 2nd edition, p 944
            for (size_t s = 1; s <= height; ++s) {
                size_t m = static_cast<size_t>(pow(2.0, s));
                double angle = 2 * M_PI / m * (invert ? -1 : 1);
                std::complex<double> step (cos(angle), sin(angle));
                for (size_t k = 0, i = 0; k < n; k += m, ++i) {
                    pool.submit(FFTTask(coefficents, k, m, step, tree, height - s, i));
                }
            }
        }
        //wait, while all will be calculated
        while (!tree.IsTreeReady()) 
            std::this_thread::yield();

        // if invert - devide by n
        if (invert)
            for (int i = 0; i < n; ++i)
                coefficents[i] /= n;
        return coefficents;
    }
}
