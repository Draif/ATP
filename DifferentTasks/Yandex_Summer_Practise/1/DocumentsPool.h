/*
 * =====================================================================================
 *
 *       Filename:  DocumentsPool.h
 *
 *    Description:  Store documents.
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

//node of one document
class Document {
public:
    Document(const std::string& doc, size_t lenght, size_t indx) : document(doc), lengthOfDocument(lenght), indexOfDocument(indx)  {}
    
    Document(const Document& another) {
        document = another.document;
        lengthOfDocument = another.lengthOfDocument;
        indexOfDocument = another.indexOfDocument;
    }
    
    Document& operator=(const Document& another) {
        if (this != &another) {
            document = another.document;
            lengthOfDocument = another.lengthOfDocument;
            indexOfDocument = another.indexOfDocument;
        }
        return *this;
    }
    // get document 
    const std::string& GetDocument() const {
        return document;
    }
    // number of Document. Each document has it own unique number
    int GetIndexOfDocument() const {
        return indexOfDocument;
    }

    size_t GetLengthOfDocument() const {
        return lengthOfDocument;
    }

private:
    std::string document;
    size_t lengthOfDocument;
    size_t indexOfDocument; // unique number of document
};

class DocumentsPool {
public:
    // used singleton, because we need only one object of DocumentsPool
    // and we should be able to access it from different classes
    static DocumentsPool& Instance() {
        static DocumentsPool docPool;
        return docPool;
    }
    
    void AddDocument(std::string doc, int length) {
        documents.push_back(Document(doc, length, documents.size()));
    }
    
    Document& GetDocument(size_t indx) {
        return documents[indx];
    }

private:
    std::vector<Document> documents;
};

