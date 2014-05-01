#ifndef TEXT_PARSER
#define TEXT_PARSER
#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include "grammatics.tab.h"
#include "indexer.h"

int yylex();
int yyerror(std::string);


class TextParser:public Indexer {
std::string sentence;
fpos_t tempPosition;
public:
    TextParser() {
        sentenceWriteTo = fopen(sentenceFileName, "w");
    }

    ~TextParser() {
        fclose(sentenceWriteTo);
    }

    void Sentence() {
        fgetpos(sentenceWriteTo, &tempPosition);
        clerk.SetPosition(tempPosition);
        fprintf(writeTo, "%s\n", sentence.c_str());
        sentence.clear();
    }

    void NewItem(const std::string& s) {
        if (sentence.empty() && s == " ") 
            return;
        sentence += s;
    }

};

class StringClerk:public Indexer {
    std::vector<fpos_t> positions;
    const int sizeOfBuffer = 1024;
    char buffer [sizeOfBuffer];
public:
    void SetPosition(fpos_t pos) {
        positions.push_back(pos);
    }
    void PrintSentence(int numberOfSentence) {
        fsetpos(sentenceWriteTo, &positions[numberOfSentence]);
        fgets(buffer, sizeOfBuffer, sentenceWriteTo);
        std::cout << buffer << std::endl;
    }
};


#endif

