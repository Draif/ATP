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


  typedef long long i64;
  i64 GetClock() {
       return clock() * 1000 / CLOCKS_PER_SEC;
  }

  class TTimePrinter {
      i64 Start;
  public:
      TTimePrinter()
          : Start(GetClock()) {
      }

      void Print() const {
          cout << "\tTime: " << this->GetTime() << " sec" << endl;
      }

      double GetTime() const {
          return (GetClock() - Start) * 0.001;
      }

      void Reset() {
          Start = GetClock();
      }
  };


template<typename T>
class TMatrix{
private:
	vector< vector<T> > mat;
public:

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

	size_t get_cols()const {
		return mat[0].size();
	}

	size_t get_rows()const {
		return mat.size();
	}

	TMatrix& operator= (const TMatrix& a){
		mat=a.mat;
		return *this;
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
		TMatrix<T> result(get_rows(), a.get_cols());
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<a.get_cols(); ++j)
				for (unsigned int k=0; k<get_cols(); ++k)
					result.mat[i][j]+=(mat[i][k]*a.mat[k][j]);
		return result;
	}

	TMatrix<T> operator+=(const TMatrix& a){
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<get_cols(); ++j)
				mat[i][j]+=a.mat[i][j];
		return *this;
	}

	TMatrix<T> operator-=(const TMatrix& a){
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<get_cols(); ++j)
				mat[i][j]-=a.mat[i][j];
		return *this;
	}

	void operator*=(const TMatrix& a){
		TMatrix<T> result(get_rows(), a.get_cols());
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<a.get_cols(); ++j)
				for (unsigned int k=0; k<get_cols(); ++k)
					result.mat[i][j]+=(mat[i][k]*a.mat[k][j]);
		*this = result;
	}

	TMatrix<T> operator*=(T a){
		for (unsigned int i=0; i<get_rows(); ++i)
			for (unsigned int j=0; j<get_cols(); ++j)
				mat[i][j]*=a;
		return *this;
	}

	bool  operator== ( TMatrix<T> another){
		if (mat.size() != another.mat.size() || mat[0].size() != another.mat.size())
			return false;
		for (unsigned int i = 0; i < another.mat.size(); ++i)
			for (unsigned int j = 0; j < another.mat[i].size(); ++j)
				if (mat[i][j] != another.mat[i][j] )
					return false;
		return true;
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
	bool flag = true;
	while (flag){
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
	if(a.get_rows() <= 128){
		return a*b;
	}
	TMatrix <T> result (a.get_cols(), a.get_rows());
	int n=a.get_cols()/2;
	TMatrix <T> temp1 (n,n);
	TMatrix <T> temp2 (n,n);

	// Matrix M1
		//A11+A22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i,j)+a.At(n+i, n+j);
		//B11+B22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i,j)+b.At(n+i, n+j);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			result.At (i, j) = result.At (i+n, j+n) = temp1.At(i, j);

	//M2
		//A21+A22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(n+i,j)+a.At(n+i, n+j);
		//B11
		b.quater(temp2,1,n);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			result.At (i+n, j+n) -= (result.At (i+n, j) = temp1.At(i, j));
	//M3
		//A11
		a.quater(temp1,1,n);
		//B12-B22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i,j+n)-b.At(n+i, n+j);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j)
			result.At (i+n, j+n) += (result.At (i, j+n) = temp1.At(i, j));

	//M4
		//A22
		a.quater(temp1, 3, n);
		//B21-B11
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i+n,j)-b.At(i, j);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j){
			result.At (i, j) += temp1.At(i, j);
			result.At (i+n, j) += temp1.At(i, j);
		}

	//M5
		//A11+A12
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i,j)+a.At(i, j+n);
		//B22
		b.quater(temp2,3,n);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j){
			result.At (i, j) -= temp1.At(i, j);
			result.At (i, j+n) += temp1.At(i, j);
		}

	//M6
		//A21-A11
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i+n,j)-a.At(i, j);
		//B11+B12
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i,j)+b.At(i, j+n);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j){
			result.At (i+n, j+n) += temp1.At(i, j);
		}
	
	//M7
		//A12-A22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp1.At(i,j)=a.At(i,j+n)-a.At(i+n, j+n);
		//B21+B22
		for (int i=0; i<n; ++i)
			for (int j=0; j<n; ++j)
				temp2.At(i,j)=b.At(i+n,j)+b.At(i+n, j+n);
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	for (int i=0; i<n; ++i)
		for (int j=0; j<n; ++j){
			result.At (i, j) += temp1.At(i, j);
		}
	return result;
}

template <typename T>
TMatrix <T> StrassenMultiply (TMatrix<T> a, TMatrix<T> b){
	if(a.get_rows() <= 128){
		return a*b;
	}
	int p2=near_power_2(a.get_cols());
	if (p2==0)
		return StrassenMultiply_for_2(a,b);
	TMatrix<T> temp1(p2, p2);
	TMatrix<T> temp2(p2, p2);
	for (int i=0; i<p2; i++)
		for(  int j=0; j<p2; ++j){
			if ((unsigned)i<a.get_cols() && (unsigned)j<a.get_cols())
				temp1.At(i,j)=a.At(i,j);
			else if (i==j)
				temp1.At(i,j)=1;
			else 
				temp1.At(i,j)=0;
		}
	for (int i=0; i<p2; i++)
		for(int j=0; j<p2; ++j){
			if ((unsigned)i<a.get_cols() && (unsigned)j<a.get_cols())
				temp2.At(i,j)=b.At(i,j);
			else if (i==j)
				temp2.At(i,j)=1;
			else 
				temp2.At(i,j)=0;
		}
	temp1 = StrassenMultiply_for_2(temp1, temp2);
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
	return rand()%10; 
} 

int main(){
	/*int size = 3;
	TMatrix <int> test1 (size, size);
	for ( int i = 0; i < size; ++i)
		for ( int j = 0; j < size; ++j)
			test1.At( i, j ) = random();

	cout << "First Test Matrix:\n" << test1 ;

	TMatrix <int> test2 (size, size);
	for ( int i = 0; i < size; ++i)
		for ( int j = 0; j < size; ++j)
			test2.At( i, j ) = random();

	cout << "Second Test Matrix:\n" << test2 ;

	cout << "First Matrix plus Second Matrix\n" << test1+test2;

	cout << "First Matrix minus Second Matrix\n" << test1-test2;

	cout << "First Matrix multiplied by Second Matrix (usual method)\n" << test1*test2;

	int number  = 5;

	cout << "First Matrix multiplied by number \n" << number << (test1 *= number);

	cout << "First Matrix now \n" << test1;
	TMatrix <int> test3;
	cout << "Checking operator = \n" << (test3 = test1 = test2);
	
	int size_tp = 5;
	test1.Resize ( size_tp, size_tp );
	for (  int i = 0; i < size_tp; ++i)
		for (int j = 0; j < size_tp; ++j)
			test1.At( i, j ) = random();
	test2.Resize ( size_tp, size_tp);
	for (int i = 0; i < size_tp; ++i)
		for (  int j = 0; j < size_tp; ++j)
			test2.At( i, j ) = random();
	cout << "First Test Matrix:\n" << test1 ;
	cout << "Second Test Matrix:\n" << test2 ;
	cout << "Strassen Multiply matrices test1 and test2\n" << StrassenMultiply(test1, test2);
	cout << "Transopsing first test matrix \n" << test1.Transpose();
	cout << "changing size\n " << test1.Resize (2,1) << test1.Resize (2, 4);

	cout << "\n\nShow multiplication\n_______________________" << endl;

	for (int k = 3; k < 5; ++k){
		cout << "Multiplication for size:" << k << endl;
		TMatrix <int> test4 (k, k);
		for ( int i = 0; i < k; ++i)
			for ( int j = 0; j < k; ++j)
				test4.At( i, j ) = random();
		TMatrix <int> test5 (k, k);
		for ( int i = 0; i < k; ++i)
			for ( int j = 0; j < k; ++j)
				test5.At( i, j ) = random();
		cout << "Multiplication using Strassen multiply:" << endl;
		cout << "Result is:\n" << ShownStrassenMultiply(test4, test5) << endl;
		cout << "Multiplication using usual method:" << endl;
		cout << test4*test5 << endl;
		cout <<( ((test4*test5) ==  StrassenMultiply( test4, test5))? "Matrices are equal\n": "Matrices are not equal\n")<< endl;
	}*/

	//measurments
	int n=16;
	TMatrix<int> a(n,n);
	TMatrix<int> b(n,n);
	a*=b;
	TTimePrinter time_tp;
	for (int i=0; i<n; ++i){
		for (int j=0; j<n; ++j){
			a.At (i, j)=random();
			b.At (i, j)=random();

		}
	}
	TMatrix <int> temp_matrix1 (n,n);
	TMatrix <int> temp_matrix2 (n,n);
	for (int k=0; k < 7; k++){
		n*=2;
		cout << "N = " << n << endl;
		a.Resize ( n, n);
		b.Resize ( n, n);
		for (int i = 0; i < n; i++){
			for (int j = (n/2); j < n; j++){
				a.At( i, j) = random();
				b.At( i, j) = random();
			}
		}

		for(int i=n/2; i < n; ++i){
			for (int j = 0 ; j < n/2; ++j){
				a.At( i, j) = random();
				b.At( i, j) = random();
			}
		}

		time_tp.Reset();
		StrassenMultiply_for_2(a,b);
		double tp_time = time_tp.GetTime();
		cout << "Strassen method for num="<<n<<" time: " << tp_time <<endl;
		time_tp.Reset();
		a*b;
		tp_time = time_tp.GetTime();
		cout << "Usual method for num="<<n<<" time: "<< tp_time <<endl;
		if ((a*b) ==  StrassenMultiply( a, b))
			cout <<"Matrices are equal\n" << endl;
		else {
			cout << "Achtung!" << endl;
			return 0;
		}
	}

	return 0;
}