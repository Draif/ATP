#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <unordered_map>
#include <set>
#include <exception>
 
class StringFinder { //realize KMP algorithm
private:
    std::string text; // original text
    std::string _template; // string template, to found in text
    std::vector<int> prefixes; // prefix function to template
	int m;
	int n;
public:
    StringFinder(const std::string& _text, const std::string& _template_) {
        text = _text;
        _template = ' ' + _template_;
		m = _template_.size();
		n = _text.size();
		prefixes.resize(m + 1);
        PrefixFunction();
    }

    bool run(std::vector<int>& results) {
        int q = 0;
        int beggining = 0; //points to the begin of new possible entry of template
        for (int i = 0; i < n; ++i) {
            while (q > 0 && _template[q + 1] != text[i]) {
                q = prefixes[q];
                if (i - q >= 0)
                    beggining = i - q + 1;
                else
                    beggining = 0;                         
            }
            if (_template[q + 1] == text[i]) {
				if (q == 0) {
					beggining = i;
				}
                ++q;
            }
            if (q == m) {
                results.push_back(beggining);
                q = prefixes[q];
                if (i - q >= 0)
                    beggining = i - q + 1;
                else 
                    beggining = 0;
            }
        }
        if (results.empty())
            return false;
        return true;
    }

private:

    void PrefixFunction() {
        prefixes[1] = 0;
        int k = 0;
        for (int q = 2; q <= m; ++q) {
            while (k > 0 && _template[k + 1] != _template[q]) {
                k = prefixes[k];
            }
            if (_template[k + 1] == _template[q]) {
                ++k;
            }
            prefixes[q] = k;
        }
    }

};
class MyException: public std::exception {
     const char* text;
public:
    MyException(const char* temp) {
        text = temp;
    }
    const char* what() const noexcept {
        return text;
    }
};

class State {
    int index;
    int common; // length of common part of text and template
    std::unordered_map<char, int> transfers;
public:
    State() {
    }
    State(const State& another) {
        *this = another;
    }
    State& operator=(const State& another) {
        index = another.index;
        common = another.common;
        transfers = another.transfers;
    }
    void SetIndex(int _index) {
        index = _index;
    }
    int GetIndex() const {
        return index;
    }
    int GetNextState(char letter) const {
        if (transfers.count(letter))
            return transfers.at(letter);
        return -1;
    }
    void SetNextState(char letter, int nextState) { //unsafe function! Can change existing state
        transfers[letter] = nextState;
    }
};


class FiniteAutomate {
    std::string dictionary;
    std::unordered_map<int, State> states;
    int startState;
    std::set<int> finishState; 
public:
    FiniteAutomate() {
    }

    FiniteAutomate(const FiniteAutomate& another) {
        *this = another;
    }

    FiniteAutomate& operator=(const FiniteAutomate& another) {
        dictionary = another.dictionary;
        states = another.states;
        startState = another.startState;
        finishState = another.finishState;
    }

    void AddDictionary(const std::string& _dictionary) {
        dictionary = _dictionary;
    }
    //return true if letter was successfully added to dictionary
    bool AddLetterToDictionary(char letter) {
        if (dictionary.find(letter) != std::string::npos) {
            dictionary.push_back(letter);
            return true;
        }
        return false;
    }

    void AddState(const State& added) {
        if (states.count(added.GetIndex()))
            throw MyException("this state is already in automate!");
        states[added.GetIndex()] = added;
    }

    void AddTransfer(const State& from, const State& to, char letter) {
        if (states.count(from.GetIndex()) || states.count(to.GetIndex()))
            throw MyException("no state!");
        states[from.GetIndex()].SetNextState(letter, to.GetIndex());
    }

    void SetStartState(const State& start) {
        if (!states.count(start.GetIndex())) {
            throw MyException("no such state!");
        }
        startState = start.GetIndex();
    }

    void SetFinishState(const State& finish) {
        if (!states.count(finish.GetIndex()))
            throw MyException("no such state!");
        finishState.insert(finish.GetIndex());
    }

    bool run(const std::string& text) {
        int currentState = startState;
        int nextState;
        for (int i = 0; i < text.size(); ++i) {
            nextState = states.at(currentState).GetNextState(text[i]);
            if (nextState < 0) {
                return false;
            } else if (finishState.count(nextState)) {
                return true;
            } else {
                currentState = nextState;
            }
        }
        return false;
    }

};

int main() {
    freopen("input.txt", "r", stdin);
    freopen("ouput.txt", "w", stdout);
    std::string text, _template;
    std::cin >> text >> _template;
    StringFinder sf (text, _template);
    std::vector<int> result;
    if (sf.run(result)) {
        for (int i = 0; i < result.size(); ++i) {
            std::cout << result[i] << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No entry in text!\n";
    }
    return 0;
}    
     

                    
