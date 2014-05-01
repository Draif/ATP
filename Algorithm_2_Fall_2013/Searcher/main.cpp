#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "indexer.h"
#include <time.h>
#include <algorithm>
#include <iomanip>
#include "Stem.h"
extern FILE* yyin;
extern int yylineno;

const std::vector<std::string> SimpleParser(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::vector<std::string> words;
    std::string temp;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
            if (temp.empty()) {
                continue;
            }
            words.push_back(temp);
            words.push_back(Stem::Instance().Stemmer(temp));
            temp.clear();
            continue;
        }
        temp += s[i];
    }
    if (!temp.empty()) {
        words.push_back(temp);
        words.push_back(Stem::Instance().Stemmer(temp));
    }
    return words;
}



int main(int argc, char **argv) {
	FILE *handle;
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " program\n";
		exit(1);
	}
	if (!(handle = fopen(argv[1], "r"))) {
		std::cerr << "Error: cannot open file\n";
		exit(1);
	}
	yyin = handle;
    std::cout << "Indexer started analyze the text. It can take time...\n";
    clock_t t = clock();
	yyparse();
    std::cout << "Analyze finished. It took: " << (double)(clock() - t) / CLOCKS_PER_SEC << " seconds\n";
    Indexer::Instance().CloseParser();
    std::cout << "Now you can enter requests. Please follow the rule: one request - one line\n";
    std::string request;
    while(std::getline(std::cin, request)) {
        //std::getline(std::cin, request);
        t = clock();
        Indexer::Instance().Query(SimpleParser(request));
        std:: cout << "Your request was processed. It took " << (double) (clock() - t) / CLOCKS_PER_SEC << " seconds\n";
    }
    return 0;
}


int yyerror(std::string s)
{   //std::cerr << "line " << yylineno; 
	//std::cerr << " Parser error: " << s << "\n";
	//exit(1);
}
