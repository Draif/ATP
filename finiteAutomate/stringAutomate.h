#pragma once
#include <iostream>
#include <string>
#include <vector>

class StringAutomate {
public:
    class State {
        int commonPart;
    public:
        State(int _commonPart) {
            commonPart = _commonPart;
        }
        State(const State& another) {
            commonPart = another.commonPart;
        }
        State& operator=(const State& another) {
            commonPart = another.commonPart;
        }
        bool operator==(const State& another) const {
            return commonPart == another.commonPart;
        }
        int GetCommonPart() const {
            return commonPart;
        }
    };
    
    StringAutomate(const std::string& temp) {
        _template = ' ' + temp;
        templateSize = temp.size();
    }

    State GetInitialState() const {
        return states.front();
    }

    State GetNextState(const State& currentState, char nextChar) {
        State nextState = currentState;
        int q = nextState.GetCommonPart();
        while ( q > 0 && _template[q + 1] != nextChar) {
            q = prefixes[q];
        }
        if (_template[q + 1] == nextChar) {
            ++q;
        }
        nextState = states[q];
        return nextState;
    }

    bool IsFinal(const State& currentState) {
        if (currentState == states.back())
            return true;
        return false;
    }

    std::vector<int> ProcessText(const std::string& text) {
        std::vector<int> results;
    	int q = 0;
        int beggining = 0; //points to the begin of new possible entry of template
        int lengthOfText = text.size();
        for (int i = 0; i < lengthOfText; ++i) {
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
            if (q == templateSize) {
                results.push_back(beggining);
                q = prefixes[q];
                if (i - q >= 0)
                    beggining = i - q + 1;
                else 
                    beggining = 0;
            }
        }
        return results;
    }

private:
    friend class StringAutomateFactory;
    std::vector<int> prefixes;
    std::vector<State> states;
    std::string _template;
    int templateSize;
};

class StringAutomateFactory {
    void InitializeAutomate(StringAutomate& newAutomate, const std::string& _template) const {
        newAutomate.prefixes.resize(_template.size() + 1);
        newAutomate.prefixes[0] = newAutomate.prefixes[1] = 0;
        newAutomate.states.push_back(StringAutomate::State(0));
        newAutomate.states.push_back(StringAutomate::State(1));
        int k = 0;
        for (int q = 2; q <= _template.size(); ++q) {
            while (k > 0 && _template[k + 1] != _template[q]) {
                k = newAutomate.prefixes[k];
            }
            if (_template[k + 1] == _template[q]) {
                ++k;
            }
            newAutomate.prefixes[q] = k;
            newAutomate.states.push_back(StringAutomate::State(q));
        }
    }

public:
    StringAutomate BuildAutomate(const std::string& _template) {
        StringAutomate returnAutomate(_template);
        return returnAutomate;
    }
};





