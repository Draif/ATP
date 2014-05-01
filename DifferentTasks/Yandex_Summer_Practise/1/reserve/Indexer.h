/*
 * =====================================================================================
 *
 *       Filename:  Indexer.h
 *
 *    Description:  Indexer of documents
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

int yylex();
int yyerror(std::string s);
extern FILE* yyin;
extern int yylineno;


class Indexer {
public:
    
    static Indexer& Instance() {
        static Indexer indexer;
        return indexer;
    }
    
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
    Indexer& operator=(const Indexer& another) { return *this;}
    TextParser parser;
    int totalNumberOfDocuments;
    double averageLength;
};
