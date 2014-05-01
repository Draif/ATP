#include <iostream>
using namespace std;

void InsertionSort(int* arr, int n){
	for(int j=1; j<n;j++){
		int key=arr[j];
		int i=j-1;
		while(i>=0 && arr[i]>key){
			arr[i+1]=arr[i];
			i--;
		}
		arr[i+1]=key;
	}
}


int main(){
int n;
	cin >> n;
	int* arr=new int [n];
	for(int i=0; i<n; ++i)
		cin >> arr[i];
	InsertionSort(arr, n);
	for(int i=0; i<n; ++i)
		cout <<arr[i] <<" ";
	delete [] arr;
	system("pause");
	return 0;
}