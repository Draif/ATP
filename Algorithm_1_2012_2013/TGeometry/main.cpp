#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

template <typename T>
class IShape;
template <typename T>
class Triangular;

template <typename T> 
class TPointG {
	T x;
	T y;
public:
	TPointG <T> (){
		x=0;
		y=0;
	}
	TPointG <T> (TPointG <T>& another){
		x=another.get_x();
		y=another.get_y();
	}

	TPointG <T> (const TPointG <T>& another){
		x=another.get_x();
		y=another.get_y();
	}

	TPointG <T> (T x_coordinate, T y_coordinate){
		x=x_coordinate;
		y=y_coordinate;
	}

	T get_x () const{
		return x;
	}
	
	T get_y () const {
		return y;
	}
	TPointG<T>& operator=(TPointG<T>& a) {
		x=a.x;
		y=a.y;
		return a;
	}

	TPointG<T>& operator=(const TPointG<T>& a) {
		x=a.x;
		y=a.y;
		return *this;
	}

	TPointG <T>& lower_p ( TPointG <T>& b)   {
		if (y>b.get_y())
			return b;
		else if (y<b.get_y())
			return *this;
		else {
			if(x>b.get_x())
				return *this;
			else 
				return b;
		}
		return b;
	}

	const TPointG <T>& lower_p ( const TPointG <T>& b) const   {
		if (y>b.get_y())
			return b;
		else if (y<b.get_y())
			return *this;
		else {
			if(x>b.get_x())
				return *this;
			else 
				return b;
		}
		return b;
	}

	bool operator==(TPointG <T> a){
		if (y==a.get_y() && x==a.get_x())
			return true;
		return false;
	}



};

template <typename T>
double length (const TPointG<T>& p1, const TPointG <T>& p2){
	return sqrt(static_cast<double>((p1.get_x()-p2.get_x())*(p1.get_x()-p2.get_x())+(p1.get_y()-p2.get_y())*(p1.get_y()-p2.get_y())));
}
template <typename T>
double cosine(const TPointG <T>& p, const TPointG <T>& min, const TPointG <T>& an){
	return abs((length(p,min)*length(min,an)))/((p.get_x()-min.get_x())*(an.get_x()-min.get_x()) + (p.get_y()-min.get_y())*(an.get_y()-min.get_y()));
}

template <typename T> 
class TSegment : public IShape<T>{
	TPointG <T> beg;
	TPointG <T> end;
public:
	TSegment(TPointG<T>& beg1, TPointG <T>& end1){
		beg=beg1.lower_p(end1);
		if (beg==beg1)
			end=end1;
		else 
			end=beg1;
	}

	TSegment(const TPointG<T>& beg1, const TPointG <T>& end1){
		beg=beg1.lower_p(end1);
		if (beg==beg1)
			end=end1;
		else 
			end=beg1;
	}
	TSegment(){
	}
	TPointG <T> end_p() {
		return end;
	}
	TPointG <T> beg_p () {
		return beg;
	}

	TPointG <T> end_p() const  {
		return end;
	}
	TPointG <T> beg_p () const  {
		return beg;
	}

	bool HasPoint(const TPointG <T> &p) const {
		double angel;
		if ((end.get_x()-beg.get_x())==0)
			angel=0.0;
		else
			angel=static_cast<double>(end.get_y()-beg.get_y())/static_cast<double>(end.get_x()-beg.get_x());
		if (angel==0.0 && end.get_x()==p.get_x() && p.get_y()>=beg.get_y() && p.get_y()<=end.get_y())
			return true;
		else if (angel==0.0)
			return false;
		if (p.get_y() < beg.get_y() || p.get_y()>end.get_y())
			return false;
		else if((angel>0 &&  p.get_x() < beg.get_x() && p.get_x()>end.get_x()) || (angel<0 &&  p.get_x() > beg.get_x() && p.get_x()<end.get_x()) )
				return false;
		if (((static_cast<double>((p.get_y()-beg.get_y()))-0.05) <= (angel*static_cast<double>((p.get_x()-beg.get_x())))) && ((static_cast<double>((p.get_y()-beg.get_y()))) >= (angel*static_cast<double>((p.get_x()-beg.get_x()))-0.05)))
			return true;
		return false;
	}
	double min_length (const TPointG<T>& p) const {
		if ((length(beg,p)-0.05 <=  length(end, p)) && (length(beg,p) >=  length(end, p)-0.05)){
			double x=static_cast<double>(beg.get_x()-end.get_x())/2;
			double y=static_cast<double>(beg.get_x()-end.get_x())/2;
			return sqrt((static_cast<double> (p.get_x())-x)*(static_cast<double> (p.get_x())-x)+(static_cast<double> (p.get_y())-y)*(static_cast<double> (p.get_y())-y));
		}
		TPointG<T> min, max;
		if (length(beg,p) < length(end, p)){
			min=beg;
			max=end;
		} else {
			min=end;
			max=beg;
		}
		double cos=cosine(p, min, max);
		if (cos<=0)
			return length(p, min);
		return (1.0-cos*cos)*length(p,min);
	}

	bool more(const TPointG <T>& p){
		double angel;
		if ((end.get_x()-beg.get_x())==0)
			angel=0.0;
		else
			angel=static_cast<double>(end.get_y()-beg.get_y())/static_cast<double>(end.get_x()-beg.get_x());
		if((beg.get_y()-end.get_y())==0){
			if (p.get_y()>=beg.get_y())
				return true;
			else
				return false;
		}
		if (angel==0.0 && end.get_x()<=p.get_x())
			return true;
		else if(angel==0.0 && end.get_x()>p.get_x())
			return false;
		if (static_cast<double>(p.get_y()) >= (angel*static_cast<double>((p.get_x()-beg.get_x())))+static_cast<double>(beg.get_y()))
			return true;
		return false;
	}
	bool more(const TPointG <T>& p)const {
		double angel;
		if ((end.get_x()-beg.get_x())==0)
			angel=0.0;
		else
			angel=static_cast<double>(end.get_y()-beg.get_y())/static_cast<double>(end.get_x()-beg.get_x());
		if((beg.get_y()-end.get_y())==0){
			if (p.get_y()>=beg.get_y())
				return true;
			else
				return false;
		}
		if (angel==0.0 && end.get_x()<=p.get_x())
			return true;
		else if(angel==0.0 && end.get_x()>p.get_x())
			return false;
		if (static_cast<double>(p.get_y()) >= (angel*static_cast<double>((p.get_x()-beg.get_x())))+static_cast<double>(beg.get_y()))
			return true;
		return false;
	}

	bool Intersects(const TSegment <T>& s) const{
		if(HasPoint(s.beg_p()) || HasPoint(s.end_p()) || s.HasPoint(beg) || s.HasPoint(end))
			return true;
		if ((more(s.beg_p())==true && more(s.end_p())==true) || (more(s.beg_p())==false && more(s.end_p())==false))
			return false;
		TSegment <T> sg1 (s.beg_p(), beg);
		TSegment <T> sg2 (s.beg_p(), end);
		if (!((sg1.more( s.end_p())==true && sg1.more(end)== true) ||  (sg1.more(s.end_p())==false && sg1.more(end)== false)))
			return false;
		if (!((sg2.more(s.end_p())==true && sg2.more(beg)== true) ||  (sg2.more(s.end_p())==false && sg2.more(beg)== false)))
			return false;
		return true;
	}

	};

template <typename T>
class Triangular : public IShape<T>{
	TPointG <T> one;
	TPointG <T> two;
	TPointG <T> three;
public:
	Triangular(TPointG <T> one1, TPointG <T> two2, TPointG <T> three3){
		one=one1;
		two=two2;
		three=three3;
	}
	
	bool HasPoint(const TPointG <T> &p) const {
		TSegment <T> seg1 (one, two);
		if (seg1.HasPoint(p))
			return true;
		if ((seg1.more(three)==true && seg1.more(p)==false)||(seg1.more(three)==false && seg1.more(p)==true))
			return false;
		TSegment <T> seg2 (one, three);
		if (seg2.HasPoint(p))
			return true;
		if ((seg2.more(two)==true && seg2.more(p)==false)||(seg2.more(two)==false && seg2.more(p)==true))
			return false;
		TSegment <T> seg3 (two, three);
		if (seg3.HasPoint(p))
			return true;
		if ((seg3.more(one)==true && seg3.more(p)==false)||(seg3.more(one)==false && seg3.more(p)==true))
			return false;
		return true;
	}
	bool Intersects(const TSegment <T> &s) const {
		TSegment<T> sg1 (one, two);
		TSegment<T> sg2 (one, three);
		TSegment<T> sg3 (two, three);
		if(sg1.Intersects(s) || sg2.Intersects(s) ||sg3.Intersects(s))
			return true;
		return false;
	}
};

template <typename T> 
class TPolygon: public IShape <T>{
	TPointG<T>* arr;
	int size;
public:
	TPolygon(TPointG<T>* a, int n){			
		arr=new TPointG<T> [n];
		for(int i=0; i<n; ++i)
			arr[i]=a[i];
		size=n;
	}
	TPolygon(TPointG<T>* a, int n, int except){		
		arr=new TPointG<T> [n-1];
		int k=0;
		for(int i=0; i<n-1; ++i, k++){
			if(except==i)
				k++;
			arr[i]=a[k];
		}
		size=n-1;
	}
			
	~TPolygon(){
		delete [] arr;
	}
	bool HasPoint(const TPointG <T> &p) const {
		for(int i=2; i<size; ++i){
			int n;
			if(i==(size-1))
				n=0;
			else
				n=i+1;
			if (!LieIntoAngel(arr[i-2], arr[i-1], arr[i], arr[n])){
				Triangular<T> tr (arr[i-2], arr[i-1], arr[i]);
				TPolygon<T> pl (arr, size, i-1);
				return (tr.HasPoint(p) || pl.HasPoint(p));
			}
		}
		for (int i=2; i<size; ++i){
			Triangular<T> tr (arr[0], arr[i-1], arr[i]);
			if (tr.HasPoint(p))
				return true;
		}
		return false;
	}
	bool Intersects(const TSegment <T> &s) const{
		for(int i=2; i<size; ++i){
			int n;
			if(i==(size-1))
				n=0;
			else
				n=i+1;
			if (!LieIntoAngel(arr[i-2], arr[i-1], arr[i], arr[n])){
				Triangular<T> tr (arr[i-2], arr[i-1], arr[i]);
				TPolygon<T> pl (arr, size, i-1);
				return (tr.Intersects(s) || pl.Intersects(s));
			}
		}
		for (int i=2; i<size; ++i){
			Triangular<T> tr (arr[0], arr[i-1], arr[i]);
			if (tr.Intersects(s))
				return true;
		}
		return false;
	}

	bool LieIntoAngel (TPointG <T> f, TPointG <T> top, TPointG <T> s,  TPointG <T>  intr) const {
		TSegment <T> sg1 (top, f);
		TSegment <T> sg2 (top, s);
		if (!((sg1.more(intr)==true && sg1.more(s)== true) ||  (sg1.more(intr)==false && sg1.more(s)== false)))
			return false;
		if (!((sg2.more(intr)==true && sg2.more(f)== true) ||  (sg2.more(intr)==false && sg2.more(f)== false)))
			return false;
		return true;
	}
};

template <typename T> 
class TCircle: public IShape <T>{
	T rad;
	TPointG<T> center;
public:
	TCircle (T r, TPointG <T> c){
		center=c;
		rad=r;
	}
	bool HasPoint(const TPointG <T> &p) const {
		if(((p.get_x()-center.get_x())*(p.get_x()-center.get_x())+(p.get_y()-center.get_y())*(p.get_y()-center.get_y()))<=rad*rad)
			return true;
		return false;
	}

	bool Intersects(const TSegment <T>& s) const{
		if ( HasPoint(s.beg_p()) || HasPoint(s.end_p()))
			return true;
		if (static_cast<double> (rad)>=s.min_length(center))
			return true;
		return false;
	}

};

template <typename T>
class IShape{
public:
	virtual bool HasPoint(const TPointG <T> &p)  const = 0;
	virtual bool Intersects(const TSegment <T> &s) const = 0;
};

int main(){
	////basic test for HasPoint
	//TPointG <double> center (0.0,0.0);
	//TCircle<double> c (5.0, center);
	//TPointG <double> p (5.0,1.0);
	//cout << c.HasPoint(p)<<endl;
	//TPointG <double> beg(2.0,-1.0);
	//TPointG <double> end (-4.0,-3.0);
	//TSegment <double> seg (beg, end);
	//TPointG <double> p2(-1.0,-2.0);
	//cout<<seg.HasPoint(p2)<<endl;

	////test for intersection for circle
	//TPointG <int> center (0,0);
	//TCircle<int> c (5, center);
	//TPointG <int> beg(5,6);
	//TPointG <int> end (5,-6);
	//TSegment <int> seg (beg, end);
	//cout<<c.Intersects(seg); 

	////Triangular
	//TPointG <int> f (0,0);
	//TPointG <int> s (1,3);
	//TPointG <int> t (3,0);
	//Triangular<int> tr (f,s,t);
	//TPointG <int> test (1,3);
	////bool flag=tr.HasPoint(test);
	//TPointG <int> ft1 (0,1);
	//TPointG <int> ft2 (4,0);
	//TSegment <int> sg (ft1, ft2);
	//bool flag=tr.Intersects(sg);


	////Intersection of segments
	//TPointG <int> f1 (0,0);
	//TPointG <int> s1 (1,3);
	//TPointG <int> f2 (0,2);
	//TPointG <int> s2 (2,4);
	//TSegment <int> seg1 (f1, s1);
	//TSegment <int> seg2 (f2, s2);
	//TPointG <int> arr [5];
	//TPointG <int> f1 (1,0);
	//arr[0]=f1;
	//TPointG <int> f2 (0,3);
	//arr[1]=f2;
	//TPointG <int> f3 (4,5);
	//arr[2]=f3;
	//TPointG <int> f4 (5,2);
	//arr[3]=f4;
	//TPointG <int> f5 (2,3);
	//arr[4]=f5;
	//TPointG <int> ft (3,2);
	//TPointG <int> ft1 (4,2);
	//TPointG <int> ft2 (6,1);
	//TSegment <int> sg (ft1, ft2);
	//TPolygon <int> pl (arr,5);
	////bool flag=pl.HasPoint(ft);
	//bool flag=pl.Intersects(sg);
	////bool flag=seg1.Intersects(seg2);

	//HasPoint for polygon


	return 0;
}