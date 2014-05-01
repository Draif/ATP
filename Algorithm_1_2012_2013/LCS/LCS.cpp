#include <stdio.h>
#include <string>
#include <string.h>

/* As if I can solve this task with maximum of 500 MB of RAM, so I decided to sacrife memory to improve perfomance. 
Because of this too, I mainly use C-style progremming*/


const int MAXLEN  = 10020; // maximum number of charecters

struct Vertex {
	int len, link;
	int next [36];
	std:: string path;
	Vertex () {	
		for (int i = 0; i < 36; ++i){
			next[i] = 0;
			if (i < 10)
				hasCharacters[i] = false;
		}
	}
	void copy (int* aNext) {
		for (int i = 0; i < 26; ++i) {
			next[i] = aNext[i];
		}
	}
	bool hasCharacters [10];
};



Vertex st [200000];
char readString [MAXLEN];
int numberOfStrings;
char resultString [MAXLEN * 10];
char dividingCharacters [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; 
int positionOfDividingCharacters [10] = {-1};
int sz, last;
char firstString [MAXLEN];
char secondString [MAXLEN];
//char finalString [MAXLEN];
int maxlen = 0;
int result;



void initAutomate (Vertex* st) {
	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}



void addCharacter (Vertex* st, char c) {
	int index = 0;
	if ( c < 'a')
		index = c - '0'; 
	else
		index = c - 'a';
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
			st[clone].copy(st[q].next);
			st[clone].link = st[q].link;
			for (; p != -1 && st[p].next[index] == q; p=st[p].link)
				st[p].next[index] = clone;
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

void DFS (int root) {
	for (int i = 0; i < 36; ++i){
		if (st[root].next[i] != 0){
			char c;
			if (i < 26)
				c = 'a' + i;
			else
				c = '0' + i - 26;
			int j = 0;
			for (; j < st[root].path.size(); ++j)
				st[st[root].next[i]].path[j] = st[root].path[j];
			st[st[root].next[i]].path.push_back(c);
			DFS(st[root].next[i]);
		}
	}
	for (int i = 26; i < 26 + numberOfStrings; ++i) {
		if (st[root].next[i] > 0) {
			st[root].hasCharacters[i - 26] = true;
			break;
		}
	}
	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < numberOfStrings; ++j)
			if (st[st[root].next[i]].hasCharacters[j])
				st[root].hasCharacters[j] = true;
	}
	bool flag = true;
	for (int i = 0; i < numberOfStrings; ++i) {
		if (st[root].hasCharacters[i] == false)
			flag = false;
	}
	if (flag) {
		if (st[root].path.size() > maxlen)
			result = root;
	}
}

/*void lcs (char* firstString, char* secondString, int n) {
	unsigned int i, j, maxLength, minLength;
	char *s, *t;
	Vertex* st;
	int v = 0,  l = 0, best = 0,  bestpos = 0;
	unsigned int firstLength = strlen(firstString);
	unsigned int secondLength = strlen(secondString);
	switch (n) {
	case 1: st = st1;
		break;
	case 2: st = st2;
		break;
	case 3: st = st3;
		break;
	case 4: st = st4;
		break;
	case 5: st = st5;
		break;
	case 6: st = st6;
		break;
	case 7: st = st7;
		break;
	case 8: st = st8;
		break;
	case 9: st = st9;
		break;
	}
	initAutomate(st);
	s = firstString;
	t = secondString;
	maxLength = firstLength;
	minLength = secondLength;

	//if ( firstLength  < secondLength) {
	//	maxLength = secondLength;
	//	minLength = firstLength;
	//	s = secondString;
	//	t = firstString;
	//} else {
	//	maxLength = firstLength;
	//	minLength = secondLength;
	//	s = firstString;
	//	t = secondString;
	//}


	for ( i = 0; i < maxLength; ++i)
		addCharacter (st, s[i]);
 	
	for (i = 0; i < minLength; ++i) {
		while (v && ! st[v].next[t[i] - 'a']) {
			v = st[v].link;
			l = st[v].len;
		}
		if (st[v].next[t[i]- 'a']) {
			v = st[v].next[t[i]- 'a'];
			++l;
		}
		if (l > best)
			best = l,  bestpos = i;
	}
	//std:: string f = t;
	//f = f.substr (bestpos-best+1, best);
	for ( i = bestpos - best + 1, j = 0 ; j < best; ++j, ++i)
		firstString[j] = t[i];
	firstString[j] = '\0';
}*/

int main () {
	int i, j = 0, p;
	scanf("%d", &numberOfStrings);
	for (i = 0; i < numberOfStrings; ++i){
		scanf("%s", readString);
		for (p = 0; readString[p] != '\0'; ++p, ++j)
			resultString[j] = readString[p];
		resultString[j] = dividingCharacters[i];
		positionOfDividingCharacters [i] = j;
		++j;
	}
	resultString[j] = '\0';
	initAutomate(st);
	for (int i = 0; i < j; ++i)
		addCharacter(st, resultString[i]);
	DFS(0);
	printf("%s", result);
	/*int k, i;
	scanf("%d", &k);
	scanf("%s", firstString);
	for (i = 1; i < k; ++i){
		scanf("%s", secondString);
		lcs(firstString, secondString, i);
	}
	if (firstString[0] == 't'){
		printf("tb");
		return 0;
	}
	printf("%s", firstString);*/
	return 0;
}