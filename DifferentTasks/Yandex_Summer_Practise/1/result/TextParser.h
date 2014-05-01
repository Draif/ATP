/*
 * =====================================================================================
 *
 *       Filename:  TextParser.h
 *
 *    Description:  Parser of text. Used in cooporation with Lexer (from flex - lex.yy.c).
 *
 *        Version:  1.0
 *        Created:  27.04.2014 23:17:45
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
#include "DocumentsPool.h"

class TextParser {
    int docNumber;
    std::string sentence;
    long long totalLenght;
    int currentLenght;
public:
    TextParser() : docNumber(0), totalLenght(0), currentLenght(0) {}
    
    //create new sentence(document) and push it in documents pool 
    void Sentence() {
        DocumentsPool::Instance().AddDocument(sentence, currentLenght);
        sentence.clear();
        totalLenght += currentLenght;
        currentLenght = 0;
        ++docNumber;
    }
    //add new character to sentence
    void NewItem(const std::string& s) {
        if (sentence.empty() && s == " ") 
            return;
        sentence += s;
        ++currentLenght;
    }

    int numberOfDocument() {
        return docNumber;
    }

    double GetAverageLength() {
        return totalLenght/docNumber;
    }
};



