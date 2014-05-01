#include <iostream>
#include <string>
#include "Indexer.h"
#include "Searcher.h"
#include <cstdio>
#include <ctime>

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

int yyerror(std::string s) {
    std::cerr << s << std::endl;
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
    std::cout << "Indexer started analyze the text. It can take time...\n";
    clock_t t = clock();
    std::cout << "Analyze finished. It took: " << (double)(clock() - t) / CLOCKS_PER_SEC << " seconds\n";
    Indexer::Instance().IndexFile(handle);
    std::cout << "Now you can enter requests. Please follow the rule: one request - one line\n";
    std::string request;
    while(std::getline(std::cin, request)) {
        t = clock();
        Searcher::Instance().Query(SimpleParser(request));
        std:: cout << "Your request was processed. It took " << (double) (clock() - t) / CLOCKS_PER_SEC << " seconds\n";
    }
    return 0;
}


