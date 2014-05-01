#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <list>
#include <queue>
using namespace std;


struct TNode {
	int num;
	unsigned char color;
	int distance;
	TNode* parent;
};

void BFS (vector < vector <unsigned char> >& matrix , vector <TNode>& vec, unsigned int v){
	queue < TNode* > qe;
	for ( unsigned int i = 0; i < vec.size(); ++i){
		if ( i != v-1){
			vec[i].color = 0;
			vec[i].distance = -1;
			vec[i].parent = 0;
		}
	}
	vec[v-1].color = 1;
	vec[v-1].distance = 0;
	vec[v-1].parent = 0;
	qe.push ( &vec[v-1]);
	TNode* tp;
	while ( ! qe.empty() ){
		tp = qe.front ();
		qe.pop ();
		for (unsigned int i = 1;  i < vec.size(); ++i){
			if (matrix[tp->num-1][i] == 1 && vec[i].color == 0){
				vec[i].color = 1;
				vec[i].distance = vec[tp->num-1].distance+1;
				vec[i].parent = &vec[tp->num-1];
				qe.push(&vec[i]);
			}
		}
		tp->color = 2;
	}
}

void clear (vector < TNode >& vec){
	for ( unsigned int i = 0; i < vec.size(); ++i){
		vec[i].color = 0;
		vec[i].distance = -1;
		vec[i].parent = 0;
	}
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
		tp.distance = 0;
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
	BFS (matrix, vec, 1);
	for (int i = 0; i < n; ++i){
		cout << "# " << i+1; 
		cout << "\t distance: " << vec[i].distance;
		if ( vec[i].parent == 0)
			cout << "\t No parent" << endl;
		else 
			cout << "\t parent: " << vec[i].parent->num << endl;
	}
	clear ( vec );
	fclose (f);
	return 0;

}
