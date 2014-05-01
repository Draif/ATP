/*
 * =====================================================================================
 *
 *       Filename:  Searcher.h
 *
 *    Description:  
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
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "WordsBor.h"
#include "DocumentsPool.h"
#include "Indexer.h"

class DocumentComparator{
public:
    bool operator()(const boost::shared_ptr<Document>& a, const boost::shared_ptr<Document>& b) const {
        return *a < *b;
    }
};


class Searcher {
public: 
    static Searcher& Instance() {
        static Searcher searcher;
        return searcher;
    }
    
    void Query(const std::vector<std::string>& query) {
        std::map<int, int> documents;
        std::vector<boost::shared_ptr<Document> > sortedVector;
        std::vector<int> docVector;
        boost::shared_ptr<LetterNode> pointer;
        std::vector<std::string>::const_iterator word = query.begin();
        for (; word != query.end(); ++word) {
            pointer = WordsBor::Instance().GetWord(*word);
            if (!pointer) 
                continue;
            docVector = pointer->GetDocuments();
            for (std::vector<int>::const_iterator p = docVector.begin(); p != docVector.end(); ++p) {
                if (documents.count(*p) == 0) {
                    documents[*p] = sortedVector.size(); 
                    sortedVector.push_back(boost::shared_ptr<Document>(&DocumentsPool::Instance().GetDocument(*p)));
                }
                sortedVector[documents[*p]]->AddWeight(countWeight(pointer, *p));
                sortedVector[documents[*p]]->AddFrequency();
            }
        }
        std::sort(sortedVector.begin(), sortedVector.end(), DocumentComparator());
        int i = 0; 
        for (std::vector<boost::shared_ptr<Document> >::const_iterator p = sortedVector.begin(); p != sortedVector.end(); ++p, ++i) 
            std::cout << i + 1 << " -\t" << (*p)->GetDocument() << std::endl;
    }

private:
    double countWeight(boost::shared_ptr<LetterNode>& ptr, int doc) {
            const double k = 2.0;
            const double b = 0.75;
            int freq = ptr->GetFrequencyInDocument(doc);
            static double averageLength = Indexer::Instance().GetAverageLength();
            static int totalNumberOfDocuments = Indexer::Instance().GetTotalNumberOfDocuments();
            return ptr->GetIDF(totalNumberOfDocuments) * freq * (k + 1) / (freq + 
            k * (1 - b + b * DocumentsPool::Instance().GetDocument(doc).GetLengthOfDocument() / averageLength));
    }          
};
