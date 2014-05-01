/*
 * =====================================================================================
 *
 *       Filename:  PrefixScan.hpp
 *
 *    Description:  Prefix scan. More information at http://en.wikipedia.org/wiki/Prefix_sum.
 *
 *        Version:  1.0
 *        Created:  21.04.2014 18:53:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), iakovenkopavel@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef PREFIX_SCAN
#define PREFIX_SCAN
#include <cmath>
#include <atomic>
#include "ThreadPool.hpp"
#include "Task_PQ.hpp"

namespace Draif {
    //this class created for guarding elements, while they are not counted yet. Represents usual binary tree.    
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

    private:
        size_t LeftChild(size_t i) {
            return 2*i + 1;
        }
        size_t RightChild(size_t i) {
            return 2*i + 2;
        }
        std::vector<std::atomic<bool>> vec;
    };
    
    //class for calculation of one node. Pushed in thread pool
    template <class TValue, class Func>
    class TreeTask {
        Func func;
        TValue& a;
        TValue& b;
        size_t height;
        size_t position;
        TreeGuard& tree;
    public:
        TreeTask(TValue& _a, TValue& _b, size_t _height, size_t _position, TreeGuard& _tree) : 
            a(_a), b(_b),  position(_position), height(_height), tree(_tree) {}
        
        int operator()() {
            while (!tree.IsReady(height, position)) {
                std::this_thread::yield();
            }
            b = func(a, b);
            tree.MarkReady(height, position);
            return 1;
        }
    };

    //class to elements, which indexes is not power of 2
    template <class TValue, class Func> 
    class UsualTask {
        Func func;
        TValue& a;
        TValue& b;
        std::atomic<int>& sem;
    public:
        UsualTask(TValue& _a, TValue& _b, std::atomic<int>& _sem) : a(_a), b(_b), sem(_sem) {}
        
        int operator()() {
            b = func(a, b);
            ++sem;
            return 1;
        }
    };
    
    

    // increase size of vector up to greater power of 2.
    // @return New size of vector
    // @params None
    template <class T>
    size_t IncreaseSize(std::vector<T>& vec) {
        size_t size = static_cast<size_t>(pow(2.0, ceil(log2(static_cast<double>(vec.size())))));
        //for (int i = vec.size(); i < size; ++i)
        //    vec.push_back(T());
        vec.resize(size);
        return vec.size();
    }

    // implements prefix scan. More imformation at http://en.wikipedia.org/wiki/Prefix_sum
    // @return TValue object, contained by result of prefix scan
    // @params std::vector<TValue> - array with objects
    //         numberOfTheads - optional parameter of working threads. By default equal to 
    //              hardware_concurency
    template <typename TValue, class Func>
    TValue PrefixScan(std::vector<TValue>& array, int numberOfThreads = 0) {
        // increase size of vector up to greater power of 2.
        size_t oldSize = array.size();
        size_t size = IncreaseSize<TValue>(array);
        
        TreeGuard tree (array.size() / 2);
        //additional interanal block - wait while all threads stop before returning result
        {
            ThreadPool<int, ConcurentQueue<ThreadPoolTask<std::function<int(void)>>>> pool(numberOfThreads);
            size_t height = static_cast<size_t> (log2(array.size()));
            int numberOfOperation = 0;
            
            //main loop for calculation
            for (size_t  i = 0; i < height; ++i) {
                size_t step = static_cast<size_t> (pow(2.0, static_cast<double>(i)));
                numberOfOperation = static_cast<int>(array.size() / (2.0*step));
                
                //submit task to thread pool
                for (int j = step - 1, k = 0; j < size; j += 2 * step, ++k) 
                    pool.submit(Draif::TreeTask<TValue, Func>(array[j], array[j + step], 
                                                            height - i - 1, k, tree));
            }
            //wait, while root would be calculated
            while (!tree.IsReady(0,0)) {};
            
            //calculate all other elements
            std::atomic<int> semaphore(0);
            for (int i = height - 2; i >= 0; --i) {
                size_t step = static_cast<size_t> (pow(2.0, static_cast<double>(i)));
                size_t numberOfIterations = static_cast<size_t> 
                                            (pow(2.0, static_cast<double>(height - i - 1)) - 1);
                for (int j = 3 * step - 1; j < array.size(); j += 2 * step) 
                    pool.submit(Draif::UsualTask<TValue, Func>(array[j - step], array[j], semaphore));
                //wait, while all elements in pool will be processed
                while (semaphore.load() != numberOfIterations);
                semaphore.store(0);
            }
        }
        array.resize(oldSize);
        return array.back();
    }
}
#endif
