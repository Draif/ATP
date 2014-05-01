#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <unordered_map>
using namespace std;

const int sizeOfDictionary = 26;

struct Vertex {
    vector<int> nextVertex;
    int patternNumber;
    int suffixLink;
    int goodSuffixLink;
    bool flag;
    int parent;
    char charFromParent;
    vector<int> autoMove;
    Vertex() {
        nextVertex.resize(-1, sizeOfDictionary);
        autoMove.resize(-1, sizeOfDictionary);
        flag = false;
        suffixLink = -1;
        parent = -1;
        charFromParent = '\0';
        goodSuffixLink = -1;
    }
    Vertex(char c, int p) {
        nextVertex.resize(-1, sizeOfDictionary);
        autoMove.resize(-1, sizeOfDictionary);
        flag = false;
        suffixLink = -1;
        parent = p;
        charFromParent = c;
        goodSuffixLink = -1;
    }
    Vertex(const Vertex& another) {
        *this = another;
    }
    Vertex& operator=(const Vertex& another) {
        nextVertex = another.nextVertex;
        flag = another.flag;
        patternNumber = another.patternNumber;
        return *this;
    }
};

class AhoCorasick {
    vector<Vertex> bohr;
    vector<string> patterns;
    unordered_map<string, bool> oldValues;
public:
    AhoCorasick() {
        bohr.push_back(Vertex());
    }

    void AddPattern(const string& pattern) {
        int num = 0;
        int temp;
        for (int i = 0; i < pattern.size(); ++i) {
            temp = pattern[i] - 'a';
            if (bohr[num].nextVertex[temp] == -1) {
                bohr.push_back(Vertex());
                bohr[num].nextVertex[temp] = bohr.size() - 1;
            }
            num = bohr[num].nextVertex[temp];
        }
        bohr[num].flag = true;
        patterns.push_back(pattern);
        bohr[num].patternNumber = patterns.size() - 1;
    }
    
    void FindAllEntries(const string& text) {
        int u = 0;
        for (int i = 0; i < text.size(); ++i) {
            u = GetAutoMove(u, text[i] - 'a');
            Check(u, i + 1);
        }
    }

private:
    void Check(int v, int i) {
        for (int u = v; u != 0; u = GetGoodSuffixLink(u)) {
            if (bohr[u].flag) {
                if (oldValues.count(patterns[bohr[u].patternNumber]) == 0) {
                    cout << patterns[bohr[u].patternNumber] << endl;
                    oldValues[patterns[bohr[u].patternNumber]] = true;
                }
            }
        }
    }

    int GetSuffixLink(int v) {
        if (bohr[v].suffixLink == -1) {
            if (v == 0 || bohr[v].parent == 0) {
                bohr[v].suffixLink = 0;
            } else {
                bohr[v].suffixLink = GetAutoMove(GetSuffixLink(bohr[v].parent), bohr[v].charFromParent);
            }
        }
        return bohr[v].suffixLink;
    }

    int GetGoodSuffixLink(int v) {
        if (bohr[v].goodSuffixLink == -1) {
            int u = GetSuffixLink(v);
            if (u == 0) {
                bohr[v].goodSuffixLink = 0;
            } else {
                bohr[v].goodSuffixLink = (bohr[u].flag) ? u : GetGoodSuffixLink(u);
            }
        }
        return bohr[v].goodSuffixLink;
    }

    int GetAutoMove(int v, char c) {
        if (bohr[v].autoMove[c] == -1) {
            if (bohr[v].nextVertex[c] != -1) {
                bohr[v].autoMove[c] = bohr[v].nextVertex[c];
            } else {
                if (v == 0) {
                    bohr[v].autoMove[c] = 0;
                } else {
                    bohr[v].autoMove[c] = GetAutoMove(GetSuffixLink(v), c);
                }
            }
        }
        return bohr[v].autoMove[c];
    }

};


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    cin >> n;
    string temp;
    AhoCorasick automate;
    for (int i = 0; i < n; ++i) {
        cin >> temp;
        automate.AddPattern(temp);
    }
    cin >> temp;
    automate.FindAllEntries(temp);
    return 0;
}
