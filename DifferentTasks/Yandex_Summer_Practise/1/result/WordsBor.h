/*
 * =====================================================================================
 *
 *       Filename:  WordsBor.h
 *
 *    Description:  Bor of words. When words have been parsered, they are going here.
 *                  I used bor because it easy way to find words.
 *
 *        Version:  1.0
 *        Created:  28.04.2014 00:10:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include "Stem.h"

//node in bor
class LetterNode {
    typedef boost::shared_ptr<LetterNode> SharedPtr;
    typedef std::map<char, SharedPtr >::const_iterator CIter;
    typedef std::map<char, SharedPtr >::iterator Iter;
public:
    LetterNode() : isWord(false), IDF(0.0) {} 

    LetterNode(std::string prev, char c) : name(prev + c), isWord(false), IDF(0.0) {}
    
    //each node can be a word - it means than it this node some word is ended. 
    // Or it can be just intermidiate node
    bool IsWord() const {
        return isWord;
    }
    // If user enter wrong word (such word that is not in bor) - we can find nearest word
    // to such "broken" word
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
    //used when we search some words in bor
    // returns zero-pointer, if we don't have node by next character
    SharedPtr GetNodeQuery(char c) {
        if (children.count(c) == 0)
            return SharedPtr ();
        return children[c];
    }
    // create new node
    // Used, when we add new word in bor
    SharedPtr GetNewNode(char c) {
        if (children.count(c) == 0) 
            children[c] = SharedPtr(new LetterNode(name, c)); 
        return children[c];
    }
    //Points, that our node is an end of some word
    void MakeAWord(int documentNumber) {
        if (!isWord) 
            isWord = true;
        
        if (docFrequency.count(documentNumber) == 0)
            docFrequency[documentNumber] = 1;
        else
            ++docFrequency[documentNumber];
        
        if (documents.empty() || documents.back() != documentNumber) 
            documents.push_back(documentNumber);
    }
    //returns number of documents, where this word occurs
    const std::vector<int>& GetDocuments() const {
        return documents;
    }
    
    // get IDF od word - used in okapi25
    double GetIDF(int numberOfDocument) {
        if (IDF > 0.0) 
            return IDF;
        else 
            IDF = std::log((numberOfDocument - documents.size() + 0.5)/(documents.size() + 0.5));
        return IDF;
    }
    // return frequency in document with index - documentNumber
    int GetFrequencyInDocument(int documentNumber) {
         return docFrequency[documentNumber];
    }
    // returns sequence of characters from root to this node
    std::string GetName() {
        return name;
    }

private: 
    std::string name; // sequence of characters from root to this node
    std::map<char, SharedPtr > children; // pointers to next nodes
    std::map<int, int> docFrequency; //frequency of this word in documents
    bool isWord;
    double IDF;
    std::vector<int> documents;//contain number of documents, where this word occurs
};

// main class of bor of words
class WordsBor {
    typedef boost::shared_ptr<LetterNode> SharedPtr;
public:
    // used singleton because we should have only one object of WordsBor 
    // and we should be able to achive to it everywhere
    static WordsBor& Instance() {
        static WordsBor bor;
        return bor;
    }
    //push new word in bor
    void AddWord(std::string word, int numberOfDocument) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        SharedPtr pointer = CreateWord(word);
        pointer->MakeAWord(numberOfDocument);
        //stem word 
        std::string stemWord =  Stem::Instance().Stemmer(word);
        if (stemWord.compare(word) != 0) {
            pointer = CreateWord(stemWord);
            pointer->MakeAWord(numberOfDocument);
        }
    }
    //Get node for query. It can be usual word, or stemmed word. 
    // if it is stemmed word - we won't print some additional information
    SharedPtr GetQueryWord(std::string word, bool isStem) {
        SharedPtr pointer = root.GetNodeQuery(word[0]);        
        SharedPtr temp;
        if (!pointer)
            return pointer;
        
        for (std::string::const_iterator p = word.begin() + 1; p != word.end(); ++p) {
            temp = pointer->GetNodeQuery(*p);
            if (temp) 
                pointer = temp;
            else {
                if (pointer->IsWord() && !isStem) 
                    std::cout << "No such word - " << word << " . Your request was changet to - " << pointer->GetName() << std::endl; 
                else if (!isStem)
                    std::cout << "No such word - " << word << std::endl;
                return pointer;
            }
        }
        //if we didn't find such word - try find nearest word to it
        if (!isStem && !pointer->IsWord()) {
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
    //create new word in bor
    SharedPtr CreateWord(std::string word) {
        SharedPtr pointer = root.GetNewNode(word[0]);
        for (std::string::const_iterator p = word.begin() + 1; p != word.end(); ++p) 
            pointer = pointer->GetNewNode(*p);
        return pointer;
    }
    
    LetterNode root;
};
