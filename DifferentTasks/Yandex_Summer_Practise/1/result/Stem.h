/*
 * =====================================================================================
 *
 *       Filename:  Stem.h
 *
 *    Description:  Stemmer for words. Use library libstemmer. More information 
 *                  about this library at http://snowball.tartarus.org/index.php.
 *
 *        Version:  1.0
 *        Created:  28.04.2014 00:16:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <string>
#include <iostream>
#include "libstemmer.h"

class Stem {
public:
    //also used singleton - just not to create many stemmers
    static Stem& Instance() {
        static Stem stem;
        return stem;
    }
    //main function of stemmer
    std::string Stemmer(std::string s) {
        //copy string
        sb_symbol* str = new unsigned char [s.size()];
        for (size_t i = 0; i < s.size(); ++i) {
            str[i] = s[i];
        }
        //if stemmer not created - create it
        if (!stemmer) {
            stemmer = sb_stemmer_new("english", "ISO_8859_1");
        }
        //stemmer itself
        const sb_symbol* symbols = sb_stemmer_stem(stemmer, str, s.size());
        delete [] str;
        return std::string((const char*)symbols);
    }
    ~Stem() {
        sb_stemmer_delete(stemmer);
    }

private:
    sb_stemmer* stemmer;
    Stem() {}
    Stem(const Stem& another) {}
    Stem& operator=(const Stem& another) {
        return *this;
    }
};
