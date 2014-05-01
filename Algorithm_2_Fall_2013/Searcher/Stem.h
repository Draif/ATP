#pragma once
#include <string>
#include <iostream>
#include "libstemmer.h"

class Stem {
public:
    static Stem& Instance() {
        static Stem stem;
        return stem;
    }
    std::string Stemmer(std::string s) {
        sb_symbol* str = new unsigned char [s.size()];
        for (int i = 0; i < s.size(); ++i) {
            str[i] = s[i];
        }
        if (!stemmer) {
            stemmer = sb_stemmer_new("english", "ISO_8859_1");
        }
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
    Stem& operator=(const Stem& another) {}
};
