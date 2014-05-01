/*
 * =====================================================================================
 *
 *       Filename:  Indexer.h
 *
 *    Description:  Main indexer of documents. Aim - index words, got from parser (TextParser.h) and 
 *                  them into bor of words, where they are stored                 
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
#include "WordsBor.h"
#include "TextParser.h"
#include "grammatics.tab.h"

//definition of variables and function from yyparse - Bison parser
int yylex();
int yyerror(std::string s);
extern FILE* yyin;
extern int yylineno;


class Indexer {
public:
    //use single-ton. We should have only one object of Indexer, and can access to it from parser
    // and syntaxis analyzer
    static Indexer& Instance() {
        static Indexer indexer;
        return indexer;
    }
    //start analyze the file
    void IndexFile(FILE* f) {
        yyin = f;
        yyparse();
    }

    TextParser& GetParser() {
        return parser;
    }
    
    void CloseParser() {
        averageLength = parser.GetAverageLength();
        totalNumberOfDocuments = parser.numberOfDocument();
    }
    //function to push word into words bor
    void IndexWord(std::string word) {
        WordsBor::Instance().AddWord(word, parser.numberOfDocument()); 
    }

    int GetTotalNumberOfDocuments() const {
        return totalNumberOfDocuments;
    }

    double GetAverageLength() const {
        return averageLength;
    }

private:
    Indexer() {}
    Indexer(const Indexer& another) {}
    Indexer& operator=(const Indexer& another) { return *this;}// write return - just because of warnings
    TextParser parser;
    int totalNumberOfDocuments;
    double averageLength;
};
