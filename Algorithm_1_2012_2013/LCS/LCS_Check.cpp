#include <iostream>
using namespace std;
char firstString [MAXLEN];
char secondString [MAXLEN];
char finalString[MAXLEN];
const int MAXLEN  = 10020; // maximum number of charecters

struct cage  {
	int num;
	int from;
};

void LCS (cage** ptr, char* s1, char* s2, int size1, int size2){
	for (int i=0; i<size1+1; i++)
		ptr[i][0].num=0;
	for (int i=1; i<size2+1; i++)
		ptr[0][i].num=0;
	for (int i=1; i<size1+1; i++)
		for (int j=1; j<size2+1; j++)
			if (s1[i-1]==s2[j-1]){
				ptr[i][j].num=ptr[i-1][j-1].num+1;
				ptr[i][j].from=2;
			}
			else
				if(ptr[i-1][j].num >= ptr[i][j-1].num){
					ptr[i][j].num=ptr[i-1][j].num;
					ptr[i][j].from=3;
				}
				else{
					ptr[i][j].num=ptr[i][j-1].num;
					ptr[i][j].from=1;
				}
}

void LCS_print (cage** ptr, char* s1, int size1, int size2, int* n){
	if(size1==0 || size2==0)
		return;
	if (ptr[size1][size2].from==2){
		LCS_print (ptr, s1, size1-1,size2-1, n);
		finalString[*n] = s1[size1-1];
		*n += 1;
	}
	else if (ptr[size1][size2].from==3)
		LCS_print(ptr, s1, size1-1,size2, n);
	else 
		LCS_print(ptr, s1, size1,size2-1, n);
}


int main(){
	int k, i, n;
	scanf("%d", &k);
	scanf("%s", firstString);
	for (i = 1; i < k; ++i){
		scanf("%s", secondString);
		int size1=strlen(firstString); // вертикаль
		int size2=strlen(secondString); // горизонталь
		cage** ptr=new cage* [size1+1];
		for (int i=0; i<size1+1; i++)
			ptr[i]=new cage [size2+1];
		n = 0;
		LCS(ptr, firstString, secondString, size1, size2);
		LCS_print(ptr, firstString, size1, size2, &n);
		firstString = finalString;
		for(int i=0; i<size1+1; i++)
			delete [] ptr[i];
		delete [] ptr;
	}
	printf("%s", firstString);	
	system("pause");
	return 0;
}