#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>


class IStringAutomate {
public:
	IStringAutomate(){}
	class State {
		int commonPart;
		public:
			State() {
			}
			State(int _commonPart) {
				commonPart = _commonPart;
			}
			State(const State& another) {
				commonPart = another.commonPart;
			}
			State& operator=(const State& another) {
				commonPart = another.commonPart;
				return *this;
			}
			bool operator==(const State& another) const {
				return commonPart == another.commonPart;
			}
			int GetCommonPart() const {
				return commonPart;
			}
	};
	IStringAutomate(const std::string&) {}
	virtual State GetInitialState() const = 0;
	virtual State GetNextState(const State&, char) const = 0;
	virtual bool IsFinal(const State&) const = 0;
	virtual std::vector<int> ProcessText(const std::string&) const = 0;
private:
	friend class StringAutomateFactory;
	friend class StringAutomate;
	std::vector<int> prefixes;
    std::vector<State> states;
};


class StringAutomate: public IStringAutomate {
public:
	StringAutomate() {
	}
    
    StringAutomate(const std::string& temp) {
        _template = " " + temp;
        templateSize = temp.size();
    }

    State GetInitialState() const {
        return states.front();
    }

    State GetNextState(const State& currentState, char nextChar) const {
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

    bool IsFinal(const State& currentState) const {
        if (currentState == states.back())
            return true;
        return false;
    }

    std::vector<int> ProcessText(const std::string& text) const {
        std::vector<int> results;
    	int q = 0;
        int beggining = 0; //points to the begin of new possible entry of template
        int lengthOfText = text.size();
        for (int i = 0; i < lengthOfText; ++i) {
            while (q > 0 && _template[q + 1] != text[i]) {
                q = prefixes[q];
                if (i - q >= 0)
                    beggining = i - q;
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
    std::string _template;
    int templateSize;
};

class StringAutomateFactory {
	void InitializeAutomate(const std::shared_ptr<IStringAutomate>& newAutomate, const std::string& _template) const {
        newAutomate->prefixes.resize(_template.size() + 1);
        newAutomate->prefixes[0] = newAutomate->prefixes[1] = 0;
        newAutomate->states.push_back(StringAutomate::State(0));
        newAutomate->states.push_back(StringAutomate::State(1));
        int k = 0;
		std::string newTemplate = " " + _template;
        for (int q = 2; q < newTemplate.size(); ++q) {
            while (k > 0 && newTemplate[k + 1] != newTemplate[q]) {
                k = newAutomate->prefixes[k];
            }
            if (newTemplate[k + 1] == newTemplate[q]) {
                ++k;
            }
            newAutomate->prefixes[q] = k;
            newAutomate->states.push_back(StringAutomate::State(q));
        }
    }

public:
    std::shared_ptr<IStringAutomate> BuildAutomate(const std::string& _template) const {
		std::shared_ptr<IStringAutomate> returnValue (new StringAutomate (_template));
        InitializeAutomate(returnValue, _template);
        return returnValue;
    }
};

