#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
const unsigned int numberOfLetters = 26;
const unsigned int sizeOfAlphabet = 3 * numberOfLetters;

class LetterNode {
public:
    LetterNode() {
        isWord = false;
        children.resize(sizeOfAlphabet);
        IDF = 0.0;
    }

    LetterNode(std::string prev, char c) {
        isWord = false;
        children.resize(sizeOfAlphabet);
        name = prev + c;
        IDF = 0.0;
    }

    bool IsWord() const {
        return isWord;
    }

    std::shared_ptr<LetterNode> GetNearestNode() {
        int tp = -1;
        for (int i = 0; i < children.size(); ++i) {
            if (children[i]) {
                if (children[i]->IsWord())
                    return children[i];
                if (tp < 0)
                    tp = i;
            }
        }
        return children[tp];
    }

    std::shared_ptr<LetterNode> GetNodeQuery(char c) {
        int index = numberOfCharacter(c);
        return children[index];
    }

    std::shared_ptr<LetterNode> GetNewNode(char c) {
        int index = numberOfCharacter(c);
        if (!children[index]) {
            children[index]= std::shared_ptr<LetterNode>(new LetterNode(name, c)); 
        }
        if (!children[index]) std::cout << "children[index] is null\n";
        return children[index];
    }
    void MakeAWord(int documentNumber) {
        if (!isWord) {
            isWord = true;
        }
        ++frequency;
        documents.push_back(documentNumber);
    }
    const std::vector<int>& GetDocuments() const {
        return documents;
    }
    void PrintNode() {
        if (isWord) {
            std::cout << "\nName: " << name << "    ";
            std::cout << "frequency: " << frequency << "    ";
            std::cout << "Documents:\n\t";
            for (int i = 0; i < documents.size(); ++i) 
                std::cout << documents[i] << " ";
            std::cout << std::endl;
        }
        for (int i = 0; i < children.size(); ++i) {
            if (children[i]) {
                children[i]->PrintNode();
            }
        }
    }

    double GetIDF(int numberOfDocument) {
        if (IDF > 0.0) {
            return IDF;
        } else {
            IDF = std::log(numberOfDocument/documents.size());
        }
        return IDF;
    }

    int GetFrequencyInDocument(int documentNumber) {
        int freq = 0;
        std::vector<int>::iterator pos = std::find(documents.begin(), documents.end(), documentNumber);
         while (*pos == documentNumber && pos != documents.end()) {
             ++freq;
             ++pos;
         }
         return freq;
    }
    std::string GetName() {
        return name;
    }
private: 
    std::string name;
    std::vector<std::shared_ptr<LetterNode>> children;
    bool isWord;
    unsigned int frequency;
    double IDF;
    std::vector<int> documents;
    int numberOfCharacter(char c) {
        int alphabet = 26;
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        } else if (c >= 'a' && c <= 'z') {
            return c - 'a' + alphabet;
        } else if (c == '\'') {
            return 2 * alphabet;
        } else if (c == '-') {
            return 2 * alphabet + 1;
        } else if (c >= '0' && c <= '9') {
            return 2 * alphabet + 1 + c - '0';
        }
    }
    char character(int numberOfCharacter) {
        if (numberOfCharacter < numberOfLetters ) {
            return (char) 'A' + numberOfCharacter;
        } else if (numberOfCharacter < 2 * numberOfLetters) {
            return (char) 'a' + numberOfCharacter - numberOfLetters;
        } else if (numberOfCharacter == 2 * numberOfLetters) {
            return '\'';
        } else if (numberOfCharacter == 2 * numberOfLetters + 1) {
            return '-';
        } else {
            return (char) numberOfCharacter + '0' - 2 * numberOfLetters - 2;
        }
    }
};

