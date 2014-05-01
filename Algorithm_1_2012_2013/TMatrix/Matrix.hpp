#pragma once
#include <iostream>
#include <vector>


template<typename T>
class Matrix {
private:
	std::vector<std::vector<T> > mat;
public:

	void quater (Matrix<T>& result, int number_of_quater, int size){
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

	Matrix(){}

	Matrix(const Matrix& a){
		*this=a;
	}

	Matrix(size_t rowCount, size_t colCount) : mat(rowCount, std::vector<T>(colCount)) {}

	Matrix(size_t rowCount, size_t colCount, T ) : mat(rowCount, std::vector<T>(colCount)) {}
	
    size_t GetColsNumber() const {
		return mat[0].size();
	}

	size_t GetRowsNumber() const {
		return mat.size();
	}
    
    std::vector<T>& GetRow(size_t indx) {
        return mat[indx];
    }

	Matrix& operator=(const Matrix& a) {
		if (this != &a)
            mat = a.mat;
		return *this;
	}

	Matrix operator+(const Matrix& a) const {
		if (a.GetColsNumber() != GetColsNumber() || a.GetRowsNumber() != GetRowsNumber()) 
			throw std::runtime_error("Error! Size of Matrixes is not сoinside!");
		Matrix result (GetRowsNumber(), GetColsNumber());
		for (unsigned int i = 0; i < GetRowsNumber(); ++i)
			for (unsigned int j = 0; j < GetColsNumber(); ++j)
				result.mat[i][j]=mat[i][j]+a.mat[i][j];
		return result;
	}

	Matrix operator-(const Matrix& a) const {
		if (a.GetColsNumber() != GetColsNumber() || a.GetRowsNumber() != GetRowsNumber())
			throw std::runtime_error("Error! Size of Matrixes is not сoinside!");
		Matrix result (GetRowsNumber(), GetColsNumber());
		for (unsigned int i=0; i<GetRowsNumber(); ++i)
			for (unsigned int j=0; j<GetColsNumber(); ++j)
				result.mat[i][j]=mat[i][j]-a.mat[i][j];
		return result;
	}
	
    Matrix operator*(T a) const {
		Matrix tp(*this);
		for (unsigned int i=0; i<GetRowsNumber(); ++i)
			for (unsigned int j=0; j<GetColsNumber(); ++j)
				tp.mat[i][j]*=a;
		return tp;
	}

	Matrix operator*(const Matrix& a) const {
		Matrix<T> result(GetRowsNumber(), a.GetColsNumber());
        for (unsigned int i=0; i<GetRowsNumber(); ++i) 
			for (unsigned int j=0; j<a.GetColsNumber(); ++j)
				for (unsigned int k=0; k<GetColsNumber(); ++k)
					result.mat[i][j]+=(mat[i][k]*a.mat[k][j]);
        return result;
	}

	Matrix operator+=(const Matrix& a) {
		for (unsigned int i = 0; i < GetRowsNumber(); ++i)
			for (unsigned int j = 0; j < GetColsNumber(); ++j)
				mat[i][j] += a.mat[i][j];
		return *this;
	}

	Matrix operator-=(const Matrix& a){
		for (unsigned int i=0; i < GetRowsNumber(); ++i)
			for (unsigned int j=0; j<GetColsNumber(); ++j)
				mat[i][j]-=a.mat[i][j];
		return *this;
	}

	void operator*=(const Matrix& a){
		Matrix<T> result(GetRowsNumber(), a.GetColsNumber());
		for (unsigned int i=0; i<GetRowsNumber(); ++i)
			for (unsigned int j=0; j<a.GetColsNumber(); ++j)
				for (unsigned int k=0; k<GetColsNumber(); ++k)
					result.mat[i][j]+=(mat[i][k]*a.mat[k][j]);
		*this = result;
	}

	Matrix<T> operator*=(T a){
		for (unsigned int i=0; i<GetRowsNumber(); ++i)
			for (unsigned int j=0; j<GetColsNumber(); ++j)
				mat[i][j]*=a;
		return *this;
	}

	bool operator==(const Matrix<T>& another) const {
		const double eps = 0.01;
        if (mat.size() != another.mat.size() || mat[0].size() != another.mat.size())
			return false;
		for (unsigned int i = 0; i < another.mat.size(); ++i) 
            for (unsigned int j = 0; j < another.mat[i].size(); ++j)
				if (!(mat[i][j] - eps <= another.mat[i][j] && mat[i][j] + eps >= another.mat[i][j]))
					return false;
		return true;
	}

	Matrix<T> Resize(size_t rowCount, size_t colCount) {
		if(rowCount>GetRowsNumber()){
			std::vector<T> tp (GetColsNumber(),0);
			for(unsigned int i=GetRowsNumber(); i<rowCount; i++)
				mat.push_back(tp);
		}
		else if (rowCount<GetRowsNumber())
			mat.resize(rowCount);	
		
        if(colCount!=GetColsNumber())
			for(unsigned int i=0; i<GetRowsNumber(); ++i)
				mat[i].resize(colCount);
		
        return *this;
	}
    
    std::vector<T>& operator[](size_t indx) {
        return mat[indx];
    }

    const std::vector<T>& operator[](size_t indx) const {
        return mat[indx];
    }
	
    const T& At(size_t row, size_t col) const{
		return mat[row][col];
	}

	T& At(size_t row, size_t col){
		return mat[row][col];
	}

	Matrix<T> Transpose(){
		T tp;
		for (unsigned int i=0; i<GetRowsNumber(); ++i)
			for (unsigned int j=i; j<GetColsNumber(); ++j)
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
    
    void SwapRows(size_t indx1, size_t indx2) {
        if (indx1 >= mat.size() || indx2 >= mat.size())
            throw std::runtime_error("SwapRows: out of range!");
        if (indx1 != indx2) 
            for (size_t i = 0; i < GetColsNumber(); ++i)
                std::swap(mat[indx1][i], mat[indx2][i]);
    }
    
    void SwapCols(size_t indx1, size_t indx2) {
        if (indx1 >= mat.size() || indx2 >= mat.size())
            throw std::runtime_error("SwapRows: out of range!");
        if (indx1 != indx2) 
            for (size_t i = 0; i < GetRowsNumber(); ++i)
                std::swap(mat[i][indx1], mat[i][indx2]);
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
Matrix<T> StrassenMultiply_for_2(Matrix<T>& a, Matrix<T>& b){
	if(a.GetRowsNumber() <= 128){
		return a*b;
	}
	Matrix <T> result (a.GetColsNumber(), a.GetRowsNumber());
	int n=a.GetColsNumber()/2;
	Matrix <T> temp1 (n,n);
	Matrix <T> temp2 (n,n);

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
Matrix <T> StrassenMultiply (Matrix<T> a, Matrix<T> b){
	if(a.GetRowsNumber() <= 128){
		return a*b;
	}
	int p2=near_power_2(a.GetColsNumber());
	if (p2==0)
		return StrassenMultiply_for_2(a,b);
	Matrix<T> temp1(p2, p2);
	Matrix<T> temp2(p2, p2);
	for (int i=0; i<p2; i++)
		for(  int j=0; j<p2; ++j){
			if ((unsigned)i<a.GetColsNumber() && (unsigned)j<a.get_cols())
				temp1.At(i,j)=a.At(i,j);
			else if (i==j)
				temp1.At(i,j)=1;
			else 
				temp1.At(i,j)=0;
		}
	for (int i=0; i<p2; i++)
		for(int j=0; j<p2; ++j){
			if ((unsigned)i<a.GetColsNumber() && (unsigned)j<a.get_cols())
				temp2.At(i,j)=b.At(i,j);
			else if (i==j)
				temp2.At(i,j)=1;
			else 
				temp2.At(i,j)=0;
		}
	temp1 = StrassenMultiply_for_2(temp1, temp2);
	temp1.resize(a.GetColsNumber());
	return temp1;
}




template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& matrix){
	int col=matrix.GetColsNumber();
	int row=matrix.GetRowsNumber();
	out<<"__________________________________\n";
	for (int i=0; i<row; ++i){
		for (int j=0; j<col; ++j)
			out << matrix.At(i,j) << '\t';
		out << std::endl;
	}
	out<<"__________________________________\n";
	return out;
}

template<typename T>
std::istream& operator>> (std::istream& in, Matrix<T>& matrix){
	int row, col;
	in>>row>>col;
	T tp;
	Matrix<T> result (row, col);
	for(int i=0; i<row; ++i){
		for(int j=0; j<col; ++j){
			in >> tp;
			result.At(i,j)=tp;
		}
	}
	matrix=result; 
	return in;
}

