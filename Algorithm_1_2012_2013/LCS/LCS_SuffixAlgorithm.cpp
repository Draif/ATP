#include <iostream>
#include <vector>
#include <string>


struct Vertex {
	int len, link;
	int next [36] = {0};
	bool path_exist [10];	
};

const int MAXLEN  = 100020; // maximum number of charecters
Vertex st [2 * MAXLEN]; // my automate
int sz, last;
char dividingCharacters [] = {1,2, 3, 4, 5, 6, 7, 8, 9, 0};
char str [MAXLEN];
char readStr [10010];

void initAutomate () {
	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}

void DFS_VISIT (int root, int from, int charSearched){
	for ( unsigned int i = 0; i < 36; ++i){
		if (from.next[i] != 0 ){
			if ( i == 26 + charSearched){
				root.path_exist[charSearched] = true;
				return;
			} else if (i >=26){
				root.path_exist[charSearched] = false;
				return;
			}
			DFS_VISIT( root, from.next[i], charSearched);
			return;
		}
	}
	root.path_exist[charSearched] = false;
}


void addCharacter (char c) {
	int index = c - 'a';
	int cur = sz++;
	st[cur].len = st[last].len + 1;
	int p;
	for (p = last; p != -1 && !st[p].next[index]; p = st[p].link)
		st[p].next[index] = cur;
	if (p == -1)
		st[cur].link = 0;
	else {
		int q = st[p].next[index];
		if (st[p].len + 1 == st[q].len)
			st[cur].link = q;
		else {
			int clone = sz++;
			st[clone].len = st[p].len + 1;
			st[clone].next = st[q].next;
			st[clone].link = st[q].link;
			for (; p!=-1 && st[p].next[c]==q; p=st[p].link)
				st[p].next[index] = clone;
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

int main () {
	int k, i
	scanf("%d", &k);
	for (i = 0; i < k; ++k) {
		scanf("%s", &readStr]);
		result += readStr + dividingCharacters[i];
	}

	std:: stack<int> sk;

	for (i = 0; i < s.size(); ++i) // build automate
		addCharacter(result[i]);

	bool flag;
	for (i = 0; i < sz; ++i){
		flag = true;
		for ( int j = 0; j < 10; ++j) {
			DFS_VISIT(i, i, j);
			flag = st[i].path_exist[j];
		}
		if (flag){
			sk.push(i);
		}
	}
	return 0;
}
