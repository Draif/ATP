#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int time;

struct TNode {
	int num;
	unsigned char color;
	int open;
	int close;
	TNode* parent;
};


void DFS_VISIT (vector < vector < TNode* > >& vec, vector <TNode*>& final, int v){
	vec[v][0]->color = 1;
	vec[v][0] -> open = ++time;
	for ( unsigned int i = 1; i < vec[v].size(); ++i){
		if (vec[v][i]->color == 0){
			vec[v][i]->parent = vec[v][0];
			DFS_VISIT( vec, final,  vec[v][i]->num - 1);
		}
	}
	vec[v][0]-> color = 2;
	vector <TNode*>::iterator pos = final.begin();
	pos = final.insert(pos, vec[v][0]);
	vec[v][0] -> close = ++time;
}


void top_sort (vector < vector < TNode* > >& vec, vector <TNode*>& final){
	for (unsigned int i = 0; i < vec.size(); ++i){
		vec[i][0]->color = 0;
		vec[i][0]->parent = 0;
		vec[i][0]->open = 0;
		vec[i][0]->close =0;
	}
	time = 0;
	for ( unsigned int i = 0; i < vec.size(); ++i)
		if ( vec [i] [0]->color == 0)
			DFS_VISIT (vec, final, i);

}


int find_number (vector <TNode*>& final, int num){
	for ( unsigned int i = 0; i < final.size(); ++i){
		if ( final[i] -> num == num )
			return i;
	}
	return 0;
}


bool checker (vector < vector < TNode* > >& vec, vector <TNode*>& final){
	bool flag = true;
	for ( unsigned int i = 0; i < vec.size(); ++i){
		int temp = find_number ( final , vec[i][0]->num);
		for (unsigned int j = 1; j < vec[i].size(); ++j){
			if ( temp > find_number ( final , vec[i][j]->num))
				flag = false;
		}
	}
	return flag;
}


int main () {
	FILE* f = fopen ("in_test.txt", "r+");
	int n , m;
	fscanf(f, "%d %d", &n, &m);
	vector < TNode > vec;
	vector <TNode*> final;
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
	top_sort (adj, final);
	// uncomment next line to check the results 
	//cout << ((checker ( adj, final )) ? "Correct": "Not correct") << endl;
	for ( unsigned int i = 0; i < adj.size(); ++i)
		cout << final[i]->num << " ";
	fclose (f);
	return 0;
}