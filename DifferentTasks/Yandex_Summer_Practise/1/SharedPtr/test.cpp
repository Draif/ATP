/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27.04.2014 18:26:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */

#include "SharedPtr.h"
#include <iostream>
class Trah {
public:
    void operator()(int* ptr) {
        delete[] ptr;
    }
};

int main() {
    SharedPtr<int, Trah> ptr (new int[10]);
    for (int i = 0; i < 10; ++i)
        ptr.Get()[i] = i;
    for (int i = 0; i < 10; ++i)
        std::cout << ptr.Get()[i] << std::endl;    
    return 0;
}


