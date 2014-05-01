#include <iostream>
#include <stack>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
using namespace std;


struct TPoint {
	int x;
	int y;
	double angel;
	TPoint operator= (TPoint& a){
		x = a.x;
		y = a.y;
		angel = a.angel;
		return *this;
	}

}point;

bool funct (const TPoint a, const TPoint b){
	return (a.angel < b.angel);
}


int triangle_area_2 (int x1, int y1, int x2, int y2, int x3, int y3) {
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

bool counter_clockwise (int x1, int y1, int x2, int y2, int x3, int y3) {
	return triangle_area_2 (x1, y1, x2, y2, x3, y3) > 0;
}

class Graham_scan {
	stack < TPoint > st;
	vector < TPoint >  arr;
	char* name_of_file;
public:

	Graham_scan (){
	}

	~Graham_scan () {
	}

	void lower () {
		TPoint tp;
		tp = arr [0];
		for ( unsigned int i = 1; i < arr.size(); ++i ){
			if ( arr[i].y < tp.y || ( arr[i].y == tp.y && arr[i].x < tp.x ) ){
				tp = arr [i];
				swap ( arr [i], arr [0]);
			}
		}
	}

	void points_sort(){
		lower ();
		for ( unsigned int i = 1; i < arr.size(); ++i ){
			arr[i].angel = atan2( arr[i].y - arr[0].y , arr[i].x - arr[0].x);
		}
		sort (arr.begin()+1, arr.end(), funct);
		for ( unsigned int i = 2; i < arr.size(); ++i ){
			if ( arr[i-1].angel- 0.000001 <= arr[i].angel && arr[i-1].angel >= arr[i].angel- 0.000001 ){
			//if (arr[i-1].angel == arr[i].angel){
				if ( (sqrt ( static_cast <double> (arr[i-1].x * arr[i-1].x + arr[i-1].y * arr[i-1].y))) <= (sqrt (static_cast <double> (arr[i].x * arr[i].x + arr[i].y * arr[i].y)))) 
					arr.erase ( arr.begin() + i - 1);
				else
					arr.erase ( arr.begin() + i);
			}
		}
	}

	void main () {
		int  n;
		cout << "Enter number of points:";
		cin >> n;
		if ( n < 3){
			cout << "ERROR! NUMBER OF POINTS IS LESS THAN 3!\n";
			return;
		}
		TPoint tp;
		cout << "Enter points:" << endl;
		for ( int i=0; i < n; ++i){
			cin >> tp.x >> tp.y;
			arr.push_back (tp);
		}
		points_sort();
		st.push (arr [0]);
		st.push (arr [1]);
		st.push (arr [2]);
		TPoint top;
		TPoint next_to_top;
		for (unsigned int i = 3; i < arr.size();  ++i){
			top = st.top();
			st.pop();
			next_to_top = st.top();
			st.push(top);
			while ( ! counter_clockwise ( next_to_top.x, next_to_top.y, top.x, top.y, arr[i].x, arr[i].y)) {
				st.pop ();
				top = st.top();
				st.pop();
				next_to_top = st.top();
				st.push(top);
			}
			st.push( arr[i] );
		}
	}

	//void print_to_file (){
	//	FILE* f = fopen("results_iakovenko.txt", "w");
	//	unsigned int n = st.size();
	//	for ( unsigned int i = 0; i < n; ++i){
	//		fprintf ( f, "%d point: (%d; %d)\n", i+1, st.top().x, st.top().y);
	//		st.pop();
	//	}
	//	fclose(f);
	//}

	void print(){
		cout << "RESULTS \n __________________________________\n";
		unsigned int n = st.size();
		for ( unsigned int i = 0; i < n; ++i){
			cout <<"point: "<< i+1<< " " << st.top().x << " " <<  st.top().y<< endl;
			st.pop();
		}
	}

};



int main(){
	Graham_scan a;
	a.main();
	a.print();
	return 0;
}
