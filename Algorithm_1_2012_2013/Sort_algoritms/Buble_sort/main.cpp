#include <iostream>
using namespace std;

void BubleSort (int* arr, int n){
	for(int i=0; i<n; ++i)
		for(int j=1; j<n-i; ++j)
			if (arr[j-1]>arr[j]){
				int temp=arr[j-1];
				arr[j-1]=arr[j];
				arr[j]=temp;
			}
}

int main(){
	int n;
	cin >> n;
	int* arr=new int [n];
	for(int i=0; i<n; ++i)
		cin >> arr[i];
	BubleSort(arr, n);
	for(int i=0; i<n; ++i)
		cout <<arr[i] <<" ";
	system("pause");
	return 0;
}
