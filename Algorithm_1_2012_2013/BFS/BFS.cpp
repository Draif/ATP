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

void BFS (vector < vector <TNode*> >& vec , unsigned int v){
	queue < TNode* > qe;
	for ( unsigned int i = 0; i < vec.size(); ++i){
		if ( i != v-1){
			vec[i][0]->color = 0;
			vec[i][0]->distance = -1;
			vec[i][0]->parent = 0;
		}
	}
	vec[v-1][0]->color = 1;
	vec[v-1][0]->distance = 0;
	vec[v-1][0]->parent = 0;
	qe.push ( vec[v-1][0]);
	TNode* tp;
	while ( ! qe.empty() ){
		tp = qe.front ();
		qe.pop ();
		for (unsigned int i = 1;  i < vec[tp->num-1].size(); ++i){
			if (vec[tp->num-1][i]->color == 0){
				vec[tp->num-1][i]->color = 1;
				vec[tp->num-1][i]->distance = vec[tp->num-1][0]->distance+1;
				vec[tp->num-1][i]->parent = vec[tp->num-1][0];
				qe.push(vec[tp->num-1][i]);
			}
		}
		tp->color = 2;
	}
}

void clear (vector < vector <TNode*> >& vec){
	for ( unsigned int i = 0; i < vec.size(); ++i){
		vec[i][0]->color = 0;
		vec[i][0]->distance = -1;
		vec[i][0]->parent = 0;
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
	vector < vector < TNode* > > adj (n);
	for (int i = 0; i < n; ++i){
		adj[i].push_back ( &vec[i] );
	}
	int from, to;
	for (int i = 0; i < m; ++i){
		fscanf(f, "%d %d", &from, &to);
		adj[from-1].push_back ( &vec[to-1] );
	}
	BFS (adj, 1);
	for (int i = 0; i < n; ++i){
		cout << "# " << i+1; 
		cout << "\t distance: " << adj[i][0]->distance;
		if ( adj[i][0]->parent == 0)
			cout << "\t No parent" << endl;
		else 
			cout << "\t parent: " << adj[i][0]->parent->num << endl;
	}
	clear ( adj );
	fclose (f);
	return 0;

}
