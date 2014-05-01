/*
 * =====================================================================================
 *
 *       Filename:  sh.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25.04.2014 17:08:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), iakovenkopavel@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <memory>
class A {
    virtual char print() {
        return '!';
    }

};

class B : public A {
public:
    int print() {
        std::cout << "I'm B!\n";
        return 1;
    }
};

 int main() {
    std::shared_ptr<B> b (new B);
    std::cout << b->print() << std::endl;
    return 1;
 }


