/*
 * =====================================================================================
 *
 *       Filename:  DocumentsPool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28.04.2014 16:13:02
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

class Document {
public:
    Document(const std::string& doc, size_t lenght, size_t indx) : document(doc), lengthOfDocument(lenght),
                                                    indexOfDocument(indx), frequency(0), weight(0.0) {}
    
    Document(const Document& another) {
        document = another.document;
        lengthOfDocument = another.lengthOfDocument;
        weight = another.weight;
        frequency = another.frequency;
    }
    
    Document& operator=(const Document& another) {
        if (this != &another) {
            document = another.document;
            lengthOfDocument = another.lengthOfDocument;
            weight = another.weight;
            frequency = another.frequency;
        }
        return *this;
    }
    
    const std::string& GetDocument() const {
        return document;
    }

    void AddWeight(double w) {
        weight += w;
    }
    
    int GetIndexOfDocument() const {
        return indexOfDocument;
    }

    size_t GetLengthOfDocument() const {
        return lengthOfDocument;
    }

    double GetWeight() const {
        return weight;
    }
    
    void AddFrequency() {
        ++frequency;
    }
    
    bool operator<(const Document& another) const {
        if (another.frequency < frequency) {
            return true;
        } else if (another.frequency == frequency && another.weight < weight) {
            return true;
        } 
        return false;
    }

private:
    std::string document;
    size_t lengthOfDocument;
    size_t indexOfDocument;
    size_t frequency;
    double weight;
};

class DocumentsPool {
public:
    static DocumentsPool& Instance() {
        static DocumentsPool docPool;
        return docPool;
    }
    
    void AddDocument(std::string doc, int length) {
        documents.push_back(Document(doc, length, documents.size()));
    }
    
    Document& GetDocument(size_t indx) {
        //should throw exception, if indx > documents.size()
        return documents[indx];
    }

private:
    std::vector<Document> documents;
};

