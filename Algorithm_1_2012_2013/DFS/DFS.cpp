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

void DFS_VISIT (vector < vector < TNode* > >& vec, int v){
	vec[v-1][0]->color = 1;
	vec[v-1][0] -> open = ++time;
	for ( unsigned int i = 1; i < vec[v-1].size(); ++i){
		if (vec[v-1][i]->color == 0){
			vec[v-1][i]->parent = vec[v-1][0];
			DFS_VISIT( vec, vec[v-1][i]->num);
		}
	}
	vec[v-1][0]->color = 2;
	vec[v-1][0] -> close = ++time;
}


void DFS (vector < vector < TNode* > >& vec){
	for (unsigned int i = 0; i < vec.size(); ++i){
		vec[i][0]->color = 0;
		vec[i][0]->parent = 0;
		vec[i][0]->open = 0;
		vec[i][0]->close =0;
	}
	time = 0;
	for ( unsigned int i = 0; i < vec.size(); ++i)
		if ( vec [i][0]->color == 0)
			DFS_VISIT (vec, i+1);

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

	vector < vector < TNode* > > adj (n);
	for (int i = 0; i < n; ++i){
		adj[i].push_back ( &vec[i] );
	}
	int from, to;
	for (int i = 0; i < m; ++i){
		fscanf(f, "%d %d", &from, &to);
		adj[from-1].push_back ( &vec[to-1] );
	}
	DFS (adj);
	for (int i = 0; i < n; ++i){
		cout << "# " << i+1; 
		cout << "\t open time: " << adj[i][0]->open;
		cout << "\t close time: " << adj[i][0]->close;
		if ( adj[i][0]->parent == 0)
			cout << "\t No parent" << endl;
		else 
			cout << "\t parent: " << adj[i][0]->parent->num << endl;
	}
	fclose (f);
	return 0;

}