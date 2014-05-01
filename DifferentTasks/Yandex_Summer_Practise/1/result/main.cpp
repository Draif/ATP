/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description: Main cpp file. Contain function int main() and simple parser - to parse queries 
 *
 *        Version:  1.0
 *        Created:  27.04.2014 15:23:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#include <iostream>
#include <string>
#include "Indexer.h"
#include "Searcher.h"
#include <cstdio>
#include <ctime>

//simple parser to parse queries
const std::vector<std::string> SimpleParser(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::vector<std::string> words;
    std::string temp;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
            if (temp.empty()) {
                continue;
            }
            words.push_back(temp);
            temp.clear();
            continue;
        }
        temp += s[i];
    }
    if (!temp.empty()) {
        words.push_back(temp);
    }
    return words;
}
//description of dunction, needs in yyparser()
int yyerror(std::string s) {
    return 1;
}

int main(int argc, char **argv) {
	FILE* handle;
	if (argc != 2) {
		std::cerr << "Usage: <name of file> \n";
		return 1;
	}
	if (!(handle = fopen(argv[1], "r"))) {
		std::cerr << "Error: cannot open file\n";
		return 1;
	}
    //analyzing of file
    std::cout << "Indexer started analyze the text. It can take time...\n";
    clock_t t = clock();
    Indexer::Instance().IndexFile(handle);
    std::cout << "Analyze finished. It took: " << static_cast<double>(clock() - t) / CLOCKS_PER_SEC << " seconds\n";
    fclose(handle);
    //query
    std::cout << "Now you can enter requests. Please follow the rule: one request - one line\n";
    std::cout << std::endl << "Request: ";
    std::string request;
    while(std::getline(std::cin, request)) {
        t = clock();
        Searcher::Instance().Query(SimpleParser(request));//process query
        std:: cout << "Your request was processed. It took " << static_cast<double>(clock() - t) / CLOCKS_PER_SEC << " seconds\n\n";
        std::cout << "Request: ";
    }
    std::cout << std::endl;
    return 0;
}


