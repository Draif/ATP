#include <iostream>
using namespace std;


int Parent(int i){
	return i/2;
}
int LeftChild(int i){
	return 2*i;
}
int RightChild(int i){
	return 2*i+1;
}

void Heapify(int* arr, int n, int i){
	int l=LeftChild(i);
	int r=RightChild(i);
	int largest;
	if (l<=n-1 && arr[l]>arr[i])
		largest=l;
	else
		largest=i;
	if (r<=n-1 && arr[r]>arr[largest])
		largest=r;
	if(largest!=i){
		swap(arr[i],arr[largest]);
		Heapify(arr,n,largest);
	}
}

void BuildHeap(int* arr, int n){
	for (int i=n/2-1;i>=0;--i)
		Heapify(arr,n,i);
}

void HeapSort(int* arr,int n){
	BuildHeap(arr, n);
	for (int i=n-1;i>0; --i){
		swap(arr[0],arr[i]);
		--n;
		Heapify(arr,n, 0);
	}
}

int main(){
	int n;
	cin >> n;
	int* arr=new int [n];
	for (int i=0; i<n; i++)
		cin >>arr[i];
	HeapSort(arr, n);
	for (int i=0; i<n; i++)
		cout << arr[i]<< " ";
	delete [] arr;
	system("pause");
	return 0;
}
