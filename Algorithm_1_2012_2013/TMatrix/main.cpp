#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
using namespace std;
int Created=0;
int Deleted=0;

struct str{
	int p;
	str (){ Created++;}
	~str (){Deleted++;}
};

template<typename T>
class TMatrix{
private:
	vector<vector<T>> mat;
public:
	TMatrix(){
	}
	TMatrix(const TMatrix & a){
		*this=a;
	}
	TMatrix(size_t rowCount, size_t colCount){
		vector<T> vec(colCount);
		for (unsigned int j=0; j<rowCount; ++j)
			mat.push_back(vec);
	}

	/*TMatrix<T> (const TMatrix& a, const TMatrix& b, size_t a_from, size_t a_to, size_t b_from, size_t b_to){
		TMatrix <T> result (a_to-a_from, b_to-b_from);
		for (size_t i=a_from; i<=a_to; ++i)
			for (size_t j=from; j<=a.col; ++j)
				for (int k=0; k<; ++k)
					result.mat[i][j]+=(a.mat[i][k]*b.mat[k][j]);
		return result;
	}*/
	
	// copy the quater of matrix 
	void quater (TMatrix<T>& result, int number_of_quater, int size){
		int rp=0;
		int cp=0;
		if( number_of_quater==1)
			rp=cp=0;
		else if (number_of_quater==3)
			rp=cp=size;
		else if (number_of_quater==4){
			rp=0;
			cp=size;
		} else if (number_of_quater==2){
			rp=size;
			cp=0;
		}
		for (int i=0; i<size; ++i)
				for (int j=0; j<size; ++j)
					result.At(i, j)=mat[i+rp][j+cp];
	}
	
	//function add little matrix to bigger one
	void put_matrix (TMatrix <T>& a, int number_of_quater, int size){
		int rp=0;
		int cp=0;
		if(number_of_quater==1)
			rp=cp=0;
		else if (number_of_quater==3)
			rp=cp=size;
		else if (number_of_quater==4){
			rp=0;
			cp=size;
		} else if (number_of_quater==2){
			rp=size;
			cp=0;
		}
		for (int i=0; i<size; ++i)
				for (int j=0; j<size; ++j)
					mat[i+rp][j+cp]=a.At(i,j);
	}
	
	size_t get_cols()const {
		return mat[0].size();
	}

	size_t get_rows()const {
		return mat.size();
	}

	void operator= (const TMatrix& a){
		mat=a.mat;
	}

	TMatrix<T> operator+(const TMatrix& a){
		if (a.get_cols()!=get_cols() || a.get_rows()!=get_rows()){
			cerr << "Error!"<<endl;
			return *this;
		}
		TMatrix<T> result (get_rows(), get_cols());
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<get_cols(); ++j)
				result.mat[i][j]=mat[i][j]+a.mat[i][j];
		return result;
	}

	TMatrix<T> operator-(const TMatrix& a){
		if (a.get_cols()!=get_cols() || a.get_rows()!=get_rows()){
			cerr << "Error!"<<endl;
			return *this;
		}
		TMatrix result (get_rows(), get_cols());
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<get_cols(); ++j)
				result.mat[i][j]=mat[i][j]-a.mat[i][j];
		return result;
	}
	TMatrix<T> operator*(T a) {
		TMatrix tp(*this);
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<get_cols(); ++j)
				tp.mat[i][j]*=a;
		return tp;
	}

	TMatrix<T> operator*(const TMatrix& a){
		if (get_cols()!=a.get_rows()){
			cerr << "Error!"<<endl;
			return *this;
		}
		TMatrix<T> result(get_rows(), a.get_cols());
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<a.get_cols(); ++j)
				for (unsigned int k=0; k<get_cols(); ++k)
					result.mat[i][j]+=(mat[i][k]*a.mat[k][j]);
		return result;
	}

	TMatrix<T> operator+=(const TMatrix& a){
		*this=*this+a;
		return *this;
	}
	TMatrix<T> operator-=(const TMatrix& a){
		*this=*this-a;
		return *this;
	}
	TMatrix<T> operator*=(const TMatrix& a){
		*this=*this*a;
		return *this;
	}
	TMatrix<T> operator*=(long long a){
		*this=*this*a;
		return *this;
	}
	TMatrix<T> Resize(size_t rowCount, size_t colCount){
		if(rowCount>get_rows()){
			vector<T> tp (get_cols(),0);
			for(unsigned int i=get_rows(); i<rowCount; i++)
				mat.push_back(tp);
		}
		else if (rowCount<get_rows()){
			mat.resize(rowCount);
		}
		if(colCount!=get_cols()){
			for(unsigned int i=0; i<get_rows(); ++i)
				mat[i].resize(colCount);
		}
		return *this;
	}

	const T& At(size_t row, size_t col) const{
		return mat[row][col];
	}
	T& At(size_t row, size_t col){
		return mat[row][col];
	}
	TMatrix<T> Transpose(){
		T tp;
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=i; j<get_cols(); ++j)
				if(i!=j){
					tp=mat[i][j];
					mat[i][j]=mat[j][i];
					mat[j][i]=tp;
				}

		return *this;
	}

	void resize(int new_size){
		mat.resize(new_size);
		for(int i=0; i<new_size; ++i)
			mat[i].resize(new_size);
	}

};

int near_power_2 (int size){
	int tp=size;
	while (true){
		if(tp==1)
			return 0;
		if (tp%2==1)
			break;
		tp/=2;
	}
	int tp1=2;
	while (tp1<size){
		tp1*=2;
	}
	return tp1;
}


// for square matrixes (power of 2)
template<typename T>
TMatrix<T> StrassenMultiply_for_2(TMatrix<T>& a, TMatrix<T>& b){
	if(a.get_rows()<129){
		return a*b;
	}
	TMatrix <T> result (a.get_cols(), a.get_rows());
	int n=a.get_cols()/2;
	TMatrix <T> temp1 (n,n);
	TMatrix <T> temp2 (n,n);
	TMatrix <T> tempMatrix (a.get_cols(), a.get_rows()); //each quater contain one matrix (M1-M7) in different steps 
	// Matrix M1
		//A11+A22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i,j)+a.At(n+i, n+j);
		//B11+B22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i,j)+b.At(n+i, n+j);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),1,n);
	//M2
		//A21+A22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(n+i,j)+a.At(n+i, n+j);
		//B11
		b.quater(temp2,1,n);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),2,n);
	//M3
		//A11
		a.quater(temp1,1,n);
		//B12-B22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i,j+n)-b.At(n+i, n+j);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),3,n);
	//M6
		//A21-A11
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i+n,j)-a.At(i, j);
		//B11+B12
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i,j)+b.At(i, j+n);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),4,n);
	//put C22 into result (M1-M2+M3+M6)
		//1-2-3-6
	for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				result.At(i+n,j+n)=tempMatrix.At(i,j)-tempMatrix.At(i+n, j)+tempMatrix.At(i+n, j+n)+tempMatrix.At(i, j+n);
	//M4
		//A22
		a.quater(temp1, 3, n);
		//B21-B11
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i+n,j)-b.At(i, j);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),4,n);
	//put C21 into result (M2+M4)
		//1-2-3-4
	for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				result.At(i+n,j)=tempMatrix.At(i+n, j)+tempMatrix.At(i, j+n);
	//M5
		//A11+A12
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i,j)+a.At(i, j+n);
		//B22
		b.quater(temp2,3,n);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),2,n);
	//put C12 into result (M3+M5)
		//1-5-3-4
	for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				result.At(i,j+n)=tempMatrix.At(i+n, j+n)+tempMatrix.At(i+n, j);
	//M7
		//A12-A22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i,j+n)-a.At(i+n, j+n);
		//B21+B22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i+n,j)+b.At(i+n, j+n);
	tempMatrix.put_matrix(StrassenMultiply(temp1, temp2),3,n);
	//put C11 into result (M1+M4-M5+M7)
		//1-5-7-4
	for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				result.At(i,j)=tempMatrix.At(i, j)+tempMatrix.At(i, j+n)-tempMatrix.At(i+n, j)+tempMatrix.At(i+n, j+n);
	return result;
}


template <typename T>
TMatrix <T> StrassenMultiply (TMatrix<T> a, TMatrix<T> b){
	int p2=near_power_2(a.get_cols());
	if (p2==0)
		return StrassenMultiply_for_2(a,b);
	TMatrix<T> temp1(p2, p2);
	TMatrix<T> temp2(p2, p2);
	for (int i=0; i<p2; i++)
		for(int j=0; j<p2; ++j){
			if (i<a.get_cols() && j<a.get_cols())
				temp1.At(i,j)=a.At(i,j);
			else if (i==j)
				temp1.At(i,j)=1;
			else 
				temp1.At(i,j)=0;
		}
	for (int i=0; i<p2; i++)
		for(int j=0; j<p2; ++j){
			if (i<a.get_cols() && j<a.get_cols())
				temp2.At(i,j)=b.At(i,j);
			else if (i==j)
				temp2.At(i,j)=1;
			else 
				temp2.At(i,j)=0;
		}
	temp1=StrassenMultiply_for_2(temp1, temp2);
	temp1.resize(a.get_cols());
	return temp1;
}




template<typename T>
ostream& operator<<(ostream& out, const TMatrix<T>& matrix){
	int col=matrix.get_cols();
	int row=matrix.get_rows();
	out<<"__________________________________\n";
	for (int i=0; i<row; ++i){
		for (int j=0; j<col; ++j)
			out<<matrix.At(i,j)<<'\t';
		out<<endl;
	}
	out<<"__________________________________\n";
	return out;
}

template<typename T>
istream& operator>> (istream& in, TMatrix<T>& matrix){
	int row, col;
	in>>row>>col;
	T tp;
	TMatrix<T> result (row, col);
	for(int i=0; i<row; ++i){
		for(int j=0; j<col; ++j){
			cin >> tp;
			result.At(i,j)=tp;
		}
	}
	matrix=result; //If I make operator= like this: TMatrix operator= (...){ ... ... ... return *this;} it goes to infinity cycle
	return in;
}



int random() {  
	return rand()%100; 
} 

int main(){
	{TMatrix<str> a(3,3);
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			a.At(i,j).p=i+j;
	}
	// created!= deleted, because I make one vector and then push_back it. I can create better methods :( There is no leaks of memory. 
	cout << Created <<endl<< Deleted<<endl;

	//measurments
	//clock_t t;
	//int n=16;
	//TMatrix<int> a(n,n);
	//TMatrix<int> b(n,n);
	//for (int i=0; i<n; ++i)
	//	for (int j=0; j<n; ++j){
	//		a.mat[i][j]=random();
	//		b.mat[i][j]=random();
	//	}
	//vector<int> tp;
	//for (int k=0; k<10; k++){
	//vector<int> tp(n*2);
	//for (int i=n; i<n*2; i++){
	//	a.mat.push_back(tp);
	//	b.mat.push_back(tp);
	//}
	//for (int i=0; i<n; i++)
	//	for (int j=n; j<n*2; j++){
	//		a.mat[i].push_back(random());
	//		b.mat[i].push_back(random());
	//	}
	//for(int i=n; i<n*2;++i)
	//	for (int j=0; j<n*2; ++j){
	//		a.mat[i][j]=random();
	//		b.mat[i][j]=random();
	//	}
	//n*=2;
	//
	//t=clock();
	//StrassenMultiply(a,b);
	//t=clock()-t;
	//cout << "Strassen method for num="<<n<<" time: "<<(float)t/CLOCKS_PER_SEC<<endl;
	//t=clock();
	//a*b;
	//t=clock()-t;
	//cout << "Usual method for num="<<n<<" time: "<<(float)t/CLOCKS_PER_SEC<<endl;
	//}


	////check all methods
	//TMatrix <int> a;
	//TMatrix <int> b;
	//cin >> a;
	//cin>> b;
	//cout<< a*b<<endl;
	//cout<< a+b<<endl;
	//cout<< a-b<<endl;
	//cout<< a*5<<endl;
	//cout<<a.Transpose()<<endl;
	//cout<<a.Resize(2,3)<<endl;
	//cout<< a.Resize(4,3)<<endl;

	////check of Strassen multiply
	//int n=9;
	//TMatrix<int> a(n,n);
	//TMatrix<int> b(n,n);
	//for(int i=0; i<n; ++i)
	//	for(int j=0; j<n; ++j){
	//		a.At(i,j)=random();
	//		b.At(i,j)=random();
	//	}
	//cout << a;
	//cout << b;
	//cout<< a*b;
	//cout <<StrassenMultiply(a,b);
	system("pause");
	return 0;
}