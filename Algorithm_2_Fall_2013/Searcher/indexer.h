#ifndef TEXT_PARSER
#define TEXT_PARSER
#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "SearcherLibrary.h"
#include "grammatics.tab.h"
#include <unordered_map>
#include <memory>
#include "Stem.h"
#include <algorithm>
int yylex();
int yyerror(std::string);
const int sizeOfBuffer = 1024;
const int numberOfTop = 10;

class FileClerk {
protected:
    const char* sentenceFileName = "sentence.txt";
    FILE* sentenceWriteTo;
};


class StringClerk:public FileClerk {
    std::vector<fpos_t> positions;
    std::vector<int> lengthOfDocument;
    char buffer [sizeOfBuffer];
public:
    void SetPosition(fpos_t pos, int len) {
        positions.push_back(pos);
        lengthOfDocument.push_back(len);
    }
    void PrintSentence(int numberOfSentence) {
        fsetpos(sentenceWriteTo, &positions[numberOfSentence]);
        fgets(buffer, sizeOfBuffer, sentenceWriteTo);
        std::cout << buffer;
    }
    int GetLenghtOfDocument(int numberOfDocument) {
        return lengthOfDocument[numberOfDocument];
    }
    void OpenFile() {
        sentenceWriteTo = fopen(sentenceFileName, "r");
    }
    void CloseFile() {
        fclose(sentenceWriteTo);
    }
    static StringClerk& Instance() {
        static StringClerk clerk;
        return clerk;
    }
private:
    StringClerk() {}
    StringClerk(const StringClerk& another) {}
    StringClerk& operator=(const StringClerk& another) {}
};

class TextParser:public FileClerk {
    int document;
    std::string sentence;
    long long totalLenght;
    int currentLenght;
    fpos_t tempPosition;
public:
    TextParser() {
        sentenceWriteTo = fopen(sentenceFileName, "w");
        document = 0;
        currentLenght = 0;
        totalLenght = 0;
    }

    void Sentence() {
        fgetpos(sentenceWriteTo, &tempPosition);
        StringClerk::Instance().SetPosition(tempPosition, currentLenght);
        fprintf(sentenceWriteTo, "%s\n", sentence.c_str());
        sentence.clear();
        totalLenght += currentLenght;
        currentLenght = 0;
        ++document;
    }

    void NewItem(const std::string& s) {
        if (sentence.empty() && s == " ") 
            return;
        sentence += s;
        ++currentLenght;
    }

    int numberOfDocument() {
        return document;
    }
    void closeFile() {
        fclose(sentenceWriteTo);
    }

    double GetAverageLength() {
        return totalLenght/document;
    }
};

class Document {
public:
    Document() {
        numberOfDocument = -1;
        weight = 0.0;
    }
    Document(int n) {
        numberOfDocument = n;
        weight = 0.0;
    }
    Document(const Document& another) {
        *this = another;
    }
    Document& operator=(const Document& another) {
        numberOfDocument = another.numberOfDocument;
        weight = another.weight;
        frequency = another.frequency;
    }
    void AddWeight(double w) {
        weight += w;
    }
    int GetNumberOfDocument() {
        return numberOfDocument;
    }
    double GetWeight() {
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
    int frequency;
    int numberOfDocument;
    double weight;
};

class Indexer {
public:
    TextParser& GetParser() {
        return parser;
    }
    void CloseParser() {
        parser.closeFile();
        averageLength = parser.GetAverageLength();
        totalNumberOfDocuments = parser.numberOfDocument();
    }
    static Indexer& Instance() {
        static Indexer indexer;
        return indexer;
    }
    void IndexWord(std::string word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        std::shared_ptr<LetterNode> temp;
        std::shared_ptr<LetterNode> pointer = root.GetNewNode(word[0]);
        for (int i = 1; i < word.size(); ++i) {
            temp = pointer->GetNewNode(word[i]);
            pointer.reset();
            pointer = std::move(temp);
        }
        pointer->MakeAWord(parser.numberOfDocument());
        word = Stem::Instance().Stemmer(word);
        pointer.reset();
        pointer = root.GetNewNode(word[0]);
        for (int i = 1; i < word.size(); ++i) {
            temp = pointer->GetNewNode(word[i]);
            pointer.reset();
            pointer = std::move(temp);
        }
        pointer->MakeAWord(parser.numberOfDocument());
    }

    void PrintTree() {
        root.PrintNode();
    }

    void Query(const std::vector<std::string>& words) {
        StringClerk::Instance().OpenFile();
        std::unordered_map<int, int> documents;
        std::vector<Document> sortedVector;
        std::shared_ptr<LetterNode> pointer;
        std::vector<int> tempVector;
        for (int i = 0; i < words.size(); ++i) {
            pointer = FindNode(words[i]);
            if (!pointer) {
                continue;
            }
            tempVector = pointer->GetDocuments();
            for (int j = 0; j < tempVector.size(); ++j) {
                if (documents.count(tempVector[j]) == 0) {
                    documents[tempVector[j]] = sortedVector.size(); 
                    sortedVector.push_back(Document(tempVector[j]));
                }
                sortedVector[documents[tempVector[j]]].AddWeight(countWeight(pointer, tempVector[j]));
                sortedVector[documents[tempVector[j]]].AddFrequency();
            }
        }
        std::sort(sortedVector.begin(), sortedVector.end());
        for (int i = 0; i < numberOfTop && i < sortedVector.size() ; ++i) {
            std::cout << i + 1 << " -\t";
            StringClerk::Instance().PrintSentence(sortedVector[i].GetNumberOfDocument());
        }
        StringClerk::Instance().CloseFile();
    }

    int TotalNumberOfDocuments() const {
        return totalNumberOfDocuments;
    }

private:
    Indexer() {}
    Indexer(const Indexer& another) {}
    Indexer& operator=(const Indexer& another) {}
    TextParser parser;
    int totalNumberOfDocuments;
    LetterNode root;
    double averageLength;
    std::shared_ptr<LetterNode> FindNode(std::string word) {
        std::shared_ptr<LetterNode> pointer = root.GetNodeQuery(word[0]);        
        std::shared_ptr<LetterNode> temp;
        if (!pointer) {
            std::cerr << "No such word - " << word << std::endl;
            return pointer;
        }
        for (int i = 1; i < word.size(); ++i) {
            temp = pointer->GetNodeQuery(word[i]);
            if (temp) {
                pointer.reset();
                pointer = temp;
                temp.reset();
            } else {
                if (pointer->IsWord()) {
                std::cerr << "No such word - " << word << " . Your request was changet to - " << pointer->GetName() << std::endl;} else {
                    std::cerr << "No such word - " << word << std::endl;
                }
                return pointer;
            }
        }
        if (!pointer->IsWord()) {
            std::cerr << "No such word - " << word << std::endl;
            while (true) {
                temp = pointer->GetNearestNode();
                pointer.reset();
                pointer = temp;
                if (!pointer)
                    return pointer;
                else if (pointer->IsWord()) {
                    std::cerr << "It was changed to - " << pointer->GetName() << std::endl;
                    return pointer;
                }
            }
        }
        return pointer;
    }

    double countWeight(const std::shared_ptr<LetterNode>& ptr, int doc) {
        const double k = 2.0;
        const double b = 0.75;
        int freq = ptr->GetFrequencyInDocument(doc);
        return ptr->GetIDF(totalNumberOfDocuments) * freq * (k + 1) / (freq + 
        k * (1 - b + b * StringClerk::Instance().GetLenghtOfDocument(doc) / averageLength));
    }           
        
};


#endif
