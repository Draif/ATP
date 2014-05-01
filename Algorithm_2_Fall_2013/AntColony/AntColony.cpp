/****************************************************
/	My programm based on ant colony optimization algorithm (ACO) with modification of elite ants and min/max pheromon
/	for more information you can see http://en.wikipedia.org/wiki/Ant_colony_optimization_algorithms
/	Pavel Iakovenko, Dolgorprudniy, 2013
/	iakovenkopavel@gmail.com
/
/
****************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
int n = 500;
const double startPheromon = 1.0;
const double minPheromon = 0.01;
const double maxPheromon = 3.0;
const int maxColonyLife = 100;
double currentLengthOfPath;
const double speedOfEvaporation = 0.7;
const double alpha = 1;
const double beta = 3;
std::deque<int> path;
std::deque<int> pathTemp;
int turn;

struct Edge {
	double distance;
	int from, to;
	double pheromon;
	double visibility;
};

struct Ant {
	int numberOfAnt;
	bool unvisitedCities[501];
	bool edgesOnPath[501][501];
	double currentPath;
	Ant() {
		currentPath = 0.0;
	}
	void clear() {
		currentPath = 0.0;
		std::deque<int>::iterator ptrEnd = pathTemp.end();
		int prev = pathTemp.front();
		for (std::deque<int>::iterator ptr = pathTemp.begin() + 1 ; ptr != ptrEnd; ++ptr) {
			if (prev < *ptr)
				edgesOnPath[prev][*ptr] = false;
			else
				edgesOnPath[*ptr][prev] = false;
			prev = *ptr;
		}
		if (pathTemp.back() < pathTemp.front()) {
			edgesOnPath[pathTemp.back()][pathTemp.front()] = false;
		} else {
			edgesOnPath[pathTemp.front()][pathTemp.back()] = false;
		}
	}
};

Edge matrix[501][501];
Ant ants[501];
bool unvisitedCities [501];
//std::vector <std::vector <Edge>> matrix;
//std::vector<Ant> ants;

double startPathCount() {
	double temp = 0;
	for (int i = 0; i < n - 1; ++i) {
		path.push_back(i);
		temp +=matrix[i][i+1].distance;
	}
	path.push_back(n - 1);
	temp += matrix[0][n-1].distance;
	return temp;
}

void updateEdges() {
	for (int i = 0; i < n; ++i) 
		for (int j = i + 1; j < n; ++j) {
				double delta = 0.0;
				for (int k = 0; k < n; ++k)
					if (ants[k].edgesOnPath[i][j])
						delta += currentLengthOfPath / ants[k].currentPath ;
				matrix[i][j].pheromon = (1 - speedOfEvaporation) * matrix[i][j].pheromon + delta;
				if (matrix[i][j].pheromon > maxPheromon)
					matrix[i][j].pheromon = maxPheromon;
				else if (matrix[i][j].pheromon < minPheromon)
					matrix[i][j].pheromon = minPheromon;
		}
}

void eliteAnt() {
	const double delta = 1.0;
	std::deque<int>::iterator ptrEnd = path.end();
	int prev = path.front();
	for (std::deque<int>::iterator ptr = path.begin() + 1 ; ptr != ptrEnd; ++ptr) {
		if (prev < *ptr) {
			matrix[prev][*ptr].pheromon += delta;
		} else {
			matrix[*ptr][prev].pheromon += delta;
		}
		prev = *ptr;
	}
	if (path.back() < path.front()) {
		matrix[path.back()][path.front()].pheromon += delta;
	} else {
		matrix[path.front()][path.back()].pheromon += delta;
	}
}


int newCityChoice (int antNumber, int currentCity) {
	double sum = 0.0;
	for (int i = 0; i < n; ++i) {
		if (unvisitedCities[i]) {
			if ( currentCity < i)
				sum += std::pow(matrix[currentCity][i].pheromon, alpha) * std::pow(matrix[currentCity][i].visibility, beta);
			else
				sum += std::pow(matrix[i][currentCity].pheromon, alpha) * std::pow(matrix[i][currentCity].visibility, beta);
		}
	}
	double probability = 0.0;
	int nextCity = ants[antNumber].numberOfAnt;
	double temp;
	for ( int i = 0; i < n; ++i) {
		if (unvisitedCities[i]) {
			if ( currentCity < i)
				temp = (std::pow(matrix[currentCity][i].pheromon, alpha) * std::pow(matrix[currentCity][i].visibility, beta))/sum;
			else
				temp = (std::pow(matrix[i][currentCity].pheromon, alpha) * std::pow(matrix[i][currentCity].visibility, beta))/sum;
			if (temp > probability) {
				probability = temp;
				nextCity = i;
			}
		}
	}
	if (currentCity < nextCity)
		ants[antNumber].edgesOnPath[currentCity][nextCity] = true;
	else 
		ants[antNumber].edgesOnPath[nextCity][currentCity] = true;
	unvisitedCities[nextCity] = false;
	if (currentCity < nextCity)
		ants[antNumber].currentPath += matrix[currentCity][nextCity].distance;
	else 
		ants[antNumber].currentPath += matrix[nextCity][currentCity].distance;
	pathTemp.push_back(nextCity);
	return nextCity;
}



void antGoNewPath (int numberOfAnt) {
	for (int i = 0; i < n; ++i) {
		if (i == numberOfAnt) {
			unvisitedCities[i] = false;
			continue;
		}
		unvisitedCities[i] = true;
	}
	int currentCity = numberOfAnt;
	for (int i = 0; i < n; ++i) {
		currentCity = newCityChoice(numberOfAnt, currentCity);
	}
	if (ants[numberOfAnt].currentPath < currentLengthOfPath) {
		currentLengthOfPath = ants[numberOfAnt].currentPath;
		std::copy(pathTemp.begin(), pathTemp.end(), path.begin());
		std::cout << "Update of shortest path! Now it equals = ";
		printf("%.12lf ", currentLengthOfPath);
		std::cout << "Turn: " << turn << std::endl;
	}	
	ants[numberOfAnt].clear();
	pathTemp.clear();
}

struct TPoint{
	int x, y;
	TPoint(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

void generateTest() {
	FILE* f = fopen("testGraph.txt", "w");
	int numberOfPoints = 8;
	std::vector<TPoint> points;
	points.push_back(TPoint(1,1));
	points.push_back(TPoint(0,3));
	points.push_back(TPoint(1,5));
	points.push_back(TPoint(4,6));
	points.push_back(TPoint(7,6));
	points.push_back(TPoint(7,3));
	points.push_back(TPoint(5,1));
	points.push_back(TPoint(6,5));
	for (int i = 0; i < numberOfPoints; ++i) {
		for (int j = 0; j < numberOfPoints; ++j){
			double tempLength = std::sqrt((points[i].x - points[j].x) * (points[i].x - points[j].x) +  (points[i].y - points[j].y) * (points[i].y - points[j].y)) ;
			fprintf(f, "%.12lf ", tempLength);
		}
		fprintf(f, "\n");
	}
	n = numberOfPoints;
	fclose(f);
}

bool checkSolution() {
	double temp = 0.0;
	bool checker [500] = {false};
	std::deque<int>::iterator ptrEnd = path.end();
	int prev = path.front();
	for (std::deque<int>::iterator ptr = path.begin() + 1 ; ptr != ptrEnd; ++ptr) {
		if (prev < *ptr) {
			temp += matrix[prev][*ptr].distance;
		} else {
			temp += matrix[*ptr][prev].distance;
		}
		if (checker[prev])
			return false;
		checker[prev] = true;
		prev = *ptr;
	}
	if (path.back() < path.front()) {
		temp += matrix[path.back()][path.front()].distance;
	} else {
		temp += matrix[path.front()][path.back()].distance;
	}
	if (checker[path.back()])
		return false;
	checker[path.back()] = true;
	if (temp <= currentLengthOfPath + 0.0000000000001 && temp >= currentLengthOfPath - 0.0000000000001) {
		for (int i = 0; i < n; ++i) {
			if (!checker[i])
				return false;
		}
		return true;
	}
	return false;
}



int main() {
	//generateTest();
	FILE* f = std::fopen("graph.txt", "r");
	std::vector<Edge> tempPush;
	Edge tempEdge;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++ j) {
			std::fscanf(f, "%lf", &matrix[i][j].distance);
			matrix[i][j].from = i;
			matrix[i][j].to = j;
			if (matrix[i][j].distance != 0) {
				matrix[i][j].visibility = 1/matrix[i][j].distance;
				matrix[i][j].pheromon = startPheromon;
			} else { 
				matrix[i][j].visibility = -1;
				matrix[i][j].pheromon = 0;
			}
		}
	}
	fclose(f);
	std::vector<bool> tempCities (n, true); 
	for (int i = 0; i < n; ++i) {
		ants[i].numberOfAnt = i;
	}
	currentLengthOfPath = startPathCount();
	for (int i = 0; i < maxColonyLife; ++i) {
		turn = i;
		std::cout << "Iteration of colony:" << i << std::endl;
		for ( int j = 0; j < n; ++j)
			antGoNewPath(j);
		for (int j = 0; j < 3; ++j) {
			eliteAnt();
		}
		updateEdges();
	}
	if (checkSolution()) {
		std::cout << "Everything is ok!\n";
		FILE* result = fopen("result.txt", "w");
		fprintf(result, "Length of path = %.12lf\n", currentLengthOfPath);
		std::deque<int>::iterator ptrEnd = path.end();
		for (std::deque<int>::iterator ptr = path.begin(); ptr != ptrEnd; ++ptr) {
			fprintf(result, "%d ", *ptr);
		}
		fclose(result);
	} else {
		std::cout << "Something going wrong!";
	}
	return 0;
}