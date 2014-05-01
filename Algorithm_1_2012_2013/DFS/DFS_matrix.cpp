#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
using namespace std;
int time;

struct TNode {
	int num;
	unsigned char color;
	int open;
	int close;
	TNode* parent;
};

void DFS_VISIT (vector < TNode >& vec, vector <vector <unsigned char> >& matrix ,  int v){
	vec[v-1].color = 1;
	vec[v-1].open = ++time;
	for ( unsigned int i = 1; i < vec.size(); ++i){
		if (matrix[v-1][i] == 1 && vec[i].color == 0 ){
			vec[i].parent = &vec[v-1];
			DFS_VISIT( vec, matrix,  i+1);
		}
	}
	vec[v-1].color = 2;
	vec[v-1].close = ++time;
}


void DFS (vector < TNode >& vec,  vector <vector <unsigned char> >& matrix){
	for (unsigned int i = 0; i < vec.size(); ++i){
		vec[i].color = 0;
		vec[i].parent = 0;
		vec[i].open = 0;
		vec[i].close =0;
	}
	time = 0;
	for ( unsigned int i = 0; i < vec.size(); ++i)
		if ( vec [i].color == 0)
			DFS_VISIT (vec, matrix, i+1);
}



int main () {
	FILE* f = fopen ("in.txt", "r+");
	int n , m;
	fscanf(f, "%d %d", &n, &m);
	vector < TNode > vec;
	TNode tp;
	for (int i = 0; i < n; ++i){
		tp.color = 0;
		tp.num = i+1;
		tp.open = 0;
		tp.close = 0;
		tp.parent = 0;
		vec.push_back (tp);
	}

	vector < vector <unsigned char> > matrix (n);
	for (int i = 0; i < n; ++i)
		matrix[i].resize(n);
	for ( int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = 0;
	int from, to;
	for (int i = 0; i < m; ++i){
		fscanf(f, "%d %d", &from, &to);
		matrix [from - 1] [to - 1] = 1;
	}
	DFS (vec, matrix);
	for (int i = 0; i < n; ++i){
		cout << "# " << i+1; 
		cout << "\t open time: " << vec[i].open;
		cout << "\t close time: " << vec[i].close;
		if ( vec[i].parent == 0)
			cout << "\t No parent" << endl;
		else 
			cout << "\t parent: " << vec[i].parent->num << endl;
	}
	fclose (f);
	return 0;
}