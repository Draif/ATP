#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "Stem.h"

class LetterNode {
    typedef boost::shared_ptr<LetterNode> SharedPtr;
    typedef std::map<char, SharedPtr >::const_iterator CIter;
    typedef std::map<char, SharedPtr >::iterator Iter;
public:
    LetterNode() : isWord(false), IDF(0.0) {} 

    LetterNode(std::string prev, char c) : isWord(false), name(prev + c), IDF(0.0) {}

    bool IsWord() const {
        return isWord;
    }

    SharedPtr GetNearestNode() {
        SharedPtr returnNode;
        if (children.empty())
            return returnNode;
        else
            returnNode = children.begin()->second;
        for (Iter p = children.begin(); p != children.end(); ++p) {
            if (p->second->IsWord())
                return p->second;
        }
        return returnNode;
    }

    SharedPtr GetNodeQuery(char c) {
        if (children.count(c) == 0)
            return SharedPtr ();
        return children[c];
    }

    SharedPtr GetNewNode(char c) {
        if (children.count(c) == 0) 
            children[c] = SharedPtr(new LetterNode(name, c)); 
        return children[c];
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
    
    /*void PrintNode() {
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
    }*/

    double GetIDF(int numberOfDocument) {
        if (IDF > 0.0) {
            return IDF;
        } else {
            IDF = std::log(numberOfDocument/documents.size());
        }
        return IDF;
    }

    int GetFrequencyInDocument(int documentNumber) const {
        int freq = 0;
        std::vector<int>::const_iterator pos = std::find(documents.begin(), documents.end(), documentNumber);
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
    std::map<char, SharedPtr > children;
    bool isWord;
    unsigned int frequency;
    double IDF;
    std::vector<int> documents;
};

class WordsBor {
    typedef boost::shared_ptr<LetterNode> SharedPtr;
public:
    static WordsBor& Instance() {
        static WordsBor bor;
        return bor;
    }

    void AddWord(std::string word, int numberOfDocument) {
        //should throw exception
        //if (word.empty())
        //    throw BohrException("Word is empty!");
        
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        SharedPtr pointer = FindWord(word);
        pointer->MakeAWord(numberOfDocument);
        word = Stem::Instance().Stemmer(word);
        pointer = FindWord(word);
        pointer->MakeAWord(numberOfDocument);
    }
    
    SharedPtr GetWord(std::string word) {
        SharedPtr pointer = SharedPtr(&root);        
        SharedPtr temp;
        for (std::string::const_iterator p = word.begin(); p != word.end(); ++p) {
            temp = pointer->GetNodeQuery(*p);
            if (temp) 
                pointer = temp;
            else {
                if (pointer->IsWord()) 
                std::cout << "No such word - " << word << " . Your request was changet to - " << pointer->GetName() << std::endl; 
                else 
                    std::cerr << "No such word - " << word << std::endl;
                return pointer;
            }
        }
        
        if (!pointer->IsWord()) {
            std::cout << "No such word - " << word << std::endl;
            while (true) {
                pointer = pointer->GetNearestNode();
                if (!pointer)
                    return pointer;
                else if (pointer->IsWord()) {
                    std::cout << "It was changed to - " << pointer->GetName() << std::endl;
                    return pointer;
                }
            }
        }
        return pointer;
    }
    
private:
    
    SharedPtr FindWord(std::string word) {
        SharedPtr pointer = SharedPtr(&root);
        for (std::string::const_iterator p = word.begin(); p != word.end(); ++p)
            pointer = pointer->GetNewNode(*p);
        return pointer;
    }

    LetterNode root;
};


