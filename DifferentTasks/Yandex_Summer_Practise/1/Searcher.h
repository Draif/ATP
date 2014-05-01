/*
 * =====================================================================================
 *
 *       Filename:  Searcher.h
 *
 *    Description: Main file, contains search engine. As ranking function I partly used
 *                  Okapi BM25. More information about it at http://en.wikipedia.org/wiki/Okapi_BM25
 *
 *        Version:  1.0
 *        Created:  28.04.2014 15:35:29
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
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include "WordsBor.h"
#include "DocumentsPool.h"
#include "Indexer.h"
#include "Stem.h"

//node, store pointer to document. This class used in search
class QueryNode {
public:
    QueryNode(Document* _doc) : doc(_doc), weight(0.0), frequency(0) {}
    
    const std::string& GetDocument() const {
        return doc->GetDocument();
    }

    void AddWeight(double w) {
        weight += w;
    }
    
    int GetIndexOfDocument() const {
        return doc->GetIndexOfDocument();
    }

    size_t GetLengthOfDocument() const {
        return doc->GetLengthOfDocument();
    }

    void AddFrequency() {
        ++frequency;
    }

    bool operator>(const QueryNode& another) const {
        if (frequency > another.frequency)
            return true;
        else if (another.frequency == frequency && weight > another.weight) 
            return true;
        return false;
    }
    
private:
    Document* doc;//pointer to document. I used pointers - because it weigh only 8 bytes
    double weight;//weight of document. Used in Okapi BM25
    int frequency;//frequency of document in query
};
    

class Searcher {
public: 
    //used singleton
    static Searcher& Instance() {
        static Searcher searcher;
        return searcher;
    }
    //main function to process query. Gets words from query    
    void Query(const std::vector<std::string>& query) {
        std::vector<QueryNode> sortedVector;
        boost::shared_ptr<LetterNode> pointer;
        std::map<int, int> documents;
        std::string stemWord;
        //this loop process every word in query 
        std::vector<std::string>::const_iterator word = query.begin();
        for (; word != query.end(); ++word) {
            //find as word itself, as it stemmed version
            pointer = WordsBor::Instance().GetQueryWord(*word, false);
            if (pointer) 
                AddDocuments(pointer, sortedVector, documents);
            pointer = WordsBor::Instance().GetQueryWord(Stem::Instance().Stemmer(*word), true);
            if (pointer) 
                AddDocuments(pointer, sortedVector, documents);
        }
        //sort documents by it's relevance
        std::sort(sortedVector.begin(), sortedVector.end(), std::greater<QueryNode>());
        //function to show results and interact with user  
        ShowResults(sortedVector);    
    }

private:

    //function to show results and interact with user  
    void ShowResults(const std::vector<QueryNode>& sortedVector) { 
        int numberOfDocument = 1, page = 1;  
        const int documentsOnPage = 10;//constant 
        std::string answer;
        double totalNumberOfDocuments = static_cast<double>(std::distance(sortedVector.begin(), sortedVector.end()));
        int numberOfPages = static_cast<int>(ceil(totalNumberOfDocuments/documentsOnPage));
        std::vector<QueryNode>::const_iterator p = sortedVector.begin(); 
        //firstly it shows first page of searched results
        std::cout << std::endl;
        for (int i = 0; i < documentsOnPage && p != sortedVector.end(); ++p, ++i, ++numberOfDocument) 
            std::cout << numberOfDocument << " -\t" << p->GetDocument() << std::endl;
        //if results are over - return
        if (p == sortedVector.end())
            return;
        
        std::cout << "****** Page " << page << " out of " <<  numberOfPages  << " ******"<< std::endl << std::endl;
        // Propose user to see more results
        //while results ends or user don't want to see them more
        for (;;) {
            //list of commands 
            std::cout << "To show next page - type \"n\". To show all results - type \"all\".";
            std::cout << " To stop watching results - type \"s\"" << std::endl;
            
            if (!std::getline(std::cin, answer))
                return;
            //switch case of chosen commands
            if (answer == "n") { // show another page of results
                std::cout << std::endl;
                for (int i = 0; i < documentsOnPage && p != sortedVector.end(); ++p, ++i, ++numberOfDocument) 
                    std::cout << numberOfDocument << " -\t" << p->GetDocument() << std::endl;
                ++page;
                std::cout << "****** Page " << page << " out of " <<  numberOfPages  << " ******"<< std::endl << std::endl;
                if (p == sortedVector.end())
                    return;
            } else if (answer == "all") { //show all results
                for (; p != sortedVector.end(); ++p, ++numberOfDocument) 
                    std::cout << numberOfDocument << " -\t" << p->GetDocument() << std::endl;
                return;
            } else if (answer == "s") //stop showing results - returns
                return;
            else // wrong command
                std::cout << "Wrong command! Try again:\n"; 
        
        }
    }
    //main function to calculate weight of single document by single word
    double countWeight(boost::shared_ptr<LetterNode>& ptr, const QueryNode& doc) {
            const double k = 2.0;
            const double b = 0.75;
            int freq = ptr->GetFrequencyInDocument(doc.GetIndexOfDocument());
            static double averageLength = Indexer::Instance().GetAverageLength();
            static int totalNumberOfDocuments = Indexer::Instance().GetTotalNumberOfDocuments();
            return ptr->GetIDF(totalNumberOfDocuments) * freq * (k + 1) / (freq + 
            k * (1 - b + b * doc.GetLengthOfDocument() / averageLength));
    }         
    // add documents, where single word from query meets
    void AddDocuments(boost::shared_ptr<LetterNode> pointer, std::vector<QueryNode>& sortedVector, std::map<int,int>& documents) {
        std::vector<int> docVector = pointer->GetDocuments();
        for (std::vector<int>::const_iterator p = docVector.begin(); p != docVector.end(); ++p) {
            if (documents.count(*p) == 0) {
                documents[*p] = sortedVector.size(); 
                sortedVector.push_back(QueryNode(&DocumentsPool::Instance().GetDocument(*p)));
            }
            sortedVector[documents[*p]].AddWeight(countWeight(pointer, sortedVector[documents[*p]]));
            sortedVector[documents[*p]].AddFrequency();
        }
    }
            
};
