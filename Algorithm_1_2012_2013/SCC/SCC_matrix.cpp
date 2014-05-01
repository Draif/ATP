#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
int time;

struct TNode {
	int num;
	unsigned char color;
	int open;
	int close;
	int added;
	bool achieved;
	TNode* parent;
};

bool myfunc (vector < TNode* > i, vector < TNode* > j){
	return ( i[0]->close > j[0]->close);
}

int search_number (vector < vector < TNode* > >& vec, TNode* v){
	for (int i = 0; i < vec.size(); ++i)
		if (vec[i][0] == v)
			return i;
	return -1;
}

void DFS_VISIT (vector < vector < TNode* > >& vec, int v){
	vec[v][0]->color = 1;
	vec[v][0] -> open = ++time;
	for ( unsigned int i = 1; i < vec[v].size(); ++i){
		if (vec[v][i]->color == 0){
			vec[v][i]->parent = vec[v][0];
			DFS_VISIT( vec, vec[v][i]->num - 1);
		}
	}
	vec[v][0]-> color = 2;
	vec[v][0] -> close = ++time;
}


void DFS (vector < vector < TNode* > >& vec){
	for (unsigned int i = 0; i < vec.size(); ++i){
		vec[i][0]->color = 0;
		vec[i][0]->parent = 0;
		vec[i][0]->open = 0;
		vec[i][0]->close = 0;
	}
	time = 0;
	for ( unsigned int i = 0; i < vec.size(); ++i)
		if ( vec [i] [0]->color == 0)
			DFS_VISIT (vec, i);
}

void Transpose (vector < vector < TNode* > >& vec){
	for (int i = 0; i < vec.size(); ++i)
		vec[i][0]->added = 0;
	vector <TNode*> :: iterator pos;
	for ( unsigned int i = 0; i < vec.size(); ++i){
		int size = vec[i].size() - vec[i][0]->added;
		pos = vec[i].begin() + 1;
		for ( int j = 1; j < size; ++j){
			int tp = search_number(vec, vec[i][1]); 
			vec[tp].push_back(vec[i][0]);
			vec[tp][0] -> added += 1;
			pos = vec[i].erase (pos);
		}
	}
}

void DFS_VISIT_REVERSE (vector < vector < TNode* > >& vec, vector< TNode* >& temp, TNode* v){
	int k = search_number(vec, v);
	bool flag = false;
	vec[k][0]->color = 1;
	for ( unsigned int i = 1; i < vec[k].size(); ++i){
		flag = true;
		if (vec[k][i]->color == 0){
			vec[k][i]->achieved = true;
			DFS_VISIT_REVERSE ( vec, temp, vec[k][i]);
		}
		if (vec[k].size() == 2 && vec[k][1]->achieved == false)
			flag = false;
		if (vec[k].size() == 2 && vec[k][1] == vec[k][0])
			flag = true;
	}
	vec[k][0]-> color = 2;
	if (flag){
		temp.push_back ( vec[k][0]);
	}
}

void DFS_REVERSE (vector < vector < TNode* > >& vec, vector < vector < TNode* > >& result){
	for (unsigned int i = 0; i < vec.size(); ++i){
		vec[i][0]->color = 0;
		vec[i][0]->achieved = false;
	}
	time = 0;
	sort (vec.begin(), vec.end(), myfunc);
	vector < TNode* > temp;
	for ( unsigned int i = 0; i < vec.size(); ++i)
		if ( vec [i] [0]->color == 0){
			DFS_VISIT_REVERSE (vec, temp, vec [i][0]);
			if ( !temp.empty() ){
				result.push_back (temp);
				temp.clear();
			}
		}
}

bool BFS (vector < vector <TNode*> >& vec , TNode* v, TNode* searched){
	queue < TNode* > qe;
	for ( unsigned int i = 0; i < vec.size(); ++i){
		if ( vec[i][0] != v){
			vec[i][0]->color = 0;
		}
		else {
			vec[i][0]->color = 1;
			qe.push (vec[i][0]);
		}
	}
	TNode* tp;
	while ( ! qe.empty() ){
		tp = qe.front ();
		qe.pop ();
		int k = search_number (vec, tp);
		for (unsigned int i = 1;  i < vec[k].size(); ++i){
			if (vec[k][i] == searched)
				return true;
			if (vec[k][i]->color == 0){
				vec[k][i]->color = 1;
				qe.push(vec[k][i]);
			}
		}
		tp->color = 2;
	}
	return false;
}

bool checker (vector < vector < TNode* > >& result, vector < vector <TNode*> >& vec){
	bool flag = true;
	for (unsigned int i = 0; i < result.size(); ++i){
		for ( unsigned int j = 0; j < result[i].size(); ++j)
			for ( unsigned int k = 0; k < result[i].size(); ++k)
				if ( k != j && !BFS(vec, result[i][j], result[i][k]))
					flag = false;
	}
	return flag;
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
	vector < vector < TNode* > > result;
	for (int i = 0; i < n; ++i){
		adj[i].push_back ( &vec[i] );
	}
	int from, to;
	for (int i = 0; i < m; ++i){
		fscanf(f, "%d %d", &from, &to);
		adj[from-1].push_back ( &vec[to-1] );
	}
	DFS (adj);
	Transpose (adj);
	DFS_REVERSE (adj, result);
	for (int i = 0; i < result.size(); ++i){
		for ( unsigned int j = 0; j < result[i].size(); ++j)
			cout << result[i][j] -> num << " ";
		cout << endl;
	}
	// uncomment this two lines, if you want check the results
	//Transpose (adj);
	//cout << checker ( result , adj);
	fclose (f);
	return 0;
}