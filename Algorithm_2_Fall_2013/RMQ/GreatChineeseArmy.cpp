#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <utility>
#include <unordered_map>
#include <algorithm>


class Fighter {
    std::string name;
    double weight;
public:
    Fighter() {
        weight = std::numeric_limits<double>::min();
    }

    Fighter (const Fighter& another) {
        *this = another;
    }

    Fighter(const std::string& _name, double _weight) {
        name = _name;
        weight = _weight;
    }

    const std::string& GetName() const {
        return name;
    }

    double GetWeight() const {
        return weight;
    }

    void SetWeight(double _weight) {
        weight = _weight;
    }

    Fighter& operator=(const Fighter& another) {
        name = another.name;
        weight = another.weight;
		return *this;
    }

    bool operator<(const Fighter& another) const  {
        if (name.compare(another.name) <= 0) 
            return true;
        return false;
    }

    bool operator==(const Fighter& another) const {
        return name == another.name;
    }

};

class SegmentTree {
    std::vector<Fighter> fighters;
public:

    SegmentTree(const std::vector<Fighter>& _fighters) {
        BuildTree(_fighters);
    }

    Fighter GreatestFighter(unsigned int left, unsigned int right) {
        double ans = std::numeric_limits<double>::min();
        Fighter greatestFighter;
        int n = fighters.size()/2;
        left += n - 1;
        right += n - 1;
        while (left <=  right) {
            if (left & 1) {
                if (fighters[left].GetWeight() > ans) {
                    ans = fighters[left].GetWeight();
                    greatestFighter = fighters[left];
                }
            }
            if (!(right & 1)) {
                if (fighters[right].GetWeight() > ans) {
                    ans = fighters[right].GetWeight();
                    greatestFighter = fighters[right];
                }
            }
            left = (left + 1) / 2;
            right = (right - 1) / 2;
        }
        return greatestFighter;
    }

    void UpdateFighter (int i, double x) {
        int n = fighters.size() / 2;
        i += n - 1;
        fighters[i].SetWeight(x);
        while (i /= 2) {
            if ( fighters[LeftChild(i)].GetWeight() > fighters[RightChild(i)].GetWeight()) {
                fighters[i] = fighters[LeftChild(i)];
            } else {
                fighters[i] = fighters[RightChild(i)];
            }
        }
    }

private:
    
    unsigned int LeftChild(unsigned int parent) {
        return 2 * parent;
    }

    unsigned int RightChild(unsigned int parent) {
        return 2 * parent + 1;
    }

    void BuildTree(const std::vector<Fighter>& _fighters) {
		int n = (1 << (static_cast<int>(std::log(_fighters.size() - 1)/std::log(2)) + 1));
        fighters.resize(2 * n);
        for (unsigned int i = 0; i < _fighters.size(); ++i) {
            fighters[i + n] = _fighters[i];
        }
        for (unsigned int i = n - 1; i > 0; --i) {
            if ( fighters[LeftChild(i)].GetWeight() > fighters[RightChild(i)].GetWeight()) {
                fighters[i] = fighters[LeftChild(i)];
            } else {
                fighters[i] = fighters[RightChild(i)];
            }
        }
    }
};

bool Boarders(const std::vector<Fighter>& fighters, std::string firstWord, \
                            std::string secondWord, int& lowerBound, int& upperBound) {
    /*int i = 0;
	if (firstWord.compare(secondWord) > 0) 
		std::swap(firstWord, secondWord);
	bool flag = false;
    while (firstWord.compare(fighters[i].GetName()) >= 0) {
        ++i;
		if (i == fighters.size()) {
			flag = true;
			break;
		}
    }
    lowerBound = i;
    i = 0;
    while (secondWord.compare(fighters[i].GetName()) >= 0) {
        ++i;
		if (i == fighters.size()) {
			if (flag) {
				return false;
			}
			break;
		}
    }
    upperBound = i - 1;
	if (upperBound < 0)
		return false;
	if (lowerBound > upperBound) 
		std::swap(lowerBound, upperBound);
	return true;
    */
    if (firstWord.compare(secondWord) > 0) {
        std::swap(firstWord, secondWord);
    }
    Fighter a (firstWord, 0.0);
    Fighter b (secondWord, 0.0);
    std::vector<Fighter>::const_iterator lowBound = std::lower_bound(fighters.begin(), fighters.end(), a);
    std::vector<Fighter>::const_iterator uppBound = std::upper_bound(fighters.begin(), fighters.end(), b);
    lowerBound = lowBound - fighters.begin();
    upperBound = uppBound - fighters.begin() - 1;
    if (lowBound == fighters.begin()) 
        ++lowerBound;
    std::cout << "Low bound: " << lowerBound << "Upper bound: " << upperBound << std::endl;
    if (lowerBound == upperBound) {
        return false;
    }
    return true;
}


int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n, m;
    std::cin >> n >> m;
    std::string tempString1, tempString2;
    double tempWeight;
    std::vector<Fighter> fighters;
    std::unordered_map<std::string, unsigned int> namesNumbers;
    for (int i = 0; i < n; ++i) {
        std::cin >> tempString1 >> tempWeight;
        fighters.push_back(Fighter(tempString1, tempWeight));
    }
    std::sort(fighters.begin(), fighters.end());
    for (int i = 0; i < fighters.size(); ++i) {
        namesNumbers[fighters[i].GetName()] = i;
    }
    SegmentTree tree (fighters);
    char tempChar;
    std::string tempName;
    for (int i = 0; i < m; ++i) {
        std::cin >> tempChar;
        if (tempChar == '!') {
            std::cin >> tempString1 >> tempString2;
			int lowerBound, upperBound;
			if (!Boarders(fighters, tempString1, tempString2, lowerBound, upperBound)) {
                std::cout << "-\n";
            } else {
                tempName = tree.GreatestFighter(lowerBound + 1, upperBound + 1).GetName();
                if (tempName.empty()) {
                    std::cout << "-\n";
                } else {
				    std::cout << tempName << std::endl;
                }
            }
        } else {
            std::cin >> tempString1 >> tempWeight;
            tree.UpdateFighter(namesNumbers[tempString1] + 1, tempWeight);
        }
    }
    return 0;
}
