#include <iostream>
using namespace std;

struct TFoo {
    int Value;
    TFoo() : Value(0) { ++Created; }
    TFoo(const TFoo &other) : Value(other.Value) { ++Created; }
    ~TFoo() { ++Deleted; }
    bool operator< (const TFoo& other)
    {
        return Value < other.Value;
    }

	bool operator< (const TFoo& other) const 
    {
        return Value < other.Value;
    }
    bool operator<= (const TFoo& other)
    {
        return Value <= other.Value;
    }
    static int Created;
    static int Deleted;
    static void PrintStats() {
        cout << "TFoo::Created = " << Created << endl
            << "TFoo::Deleted = " << Deleted << endl;
    }
	bool operator== (const TFoo& a){
		return ( Value == a.Value);
	}
};


int TFoo::Created = 0;
int TFoo::Deleted = 0;


template <typename T>
class TVector{
public:
	class iterator{
		friend class TVector;
	public:
		iterator(T* t):el(t){}
		iterator():el(0){
		};
	
		iterator& operator= (iterator another){
			el = another.el;
			return *this;
		}

		iterator& operator++(){
			el++;
			return *this;
		}

		iterator& operator++(int){
			T* temp = el;
			el++;
			return iterator(temp);
		}

		iterator& operator--(){
			el--;
			return *this;
		}

		iterator& operator--(int){
			T* temp = el;
			el--;
			return iterator(temp);
		}

		T& operator *(){
			return *el;
		}

		T* operator->(){
			return el;
		}

		bool operator== (iterator& i){
			return (el == i.el ? true:false);
		}

		bool operator!= (iterator& i){
			return !(*this == i);
		}

		T* get(){
			return el;
		}

		int operator-(iterator tp){
			T* t=el;
			int i=0;
			for(; t!=tp.get(); t++, i++);
			return i;
		}

		iterator operator+(int n){
			for (int i=0; i<n; ++i)
				++*this;
			return *this;
		}

	private:
		T* el;
	};
	class const_iterator{
		friend class TVector;
	public:
		const_iterator(T *t):el(t){}
		const_iterator():el(0){
		};

		const_iterator& operator++(){
			el++;
			return *this;
		};

		const_iterator& operator++(int){
			T* temp = el;
			el++;
			return const_iterator(temp);
		};

		const_iterator& operator--(){
			el--;
			return *this;
		}

		const_iterator& operator--(int){
			T* temp = el;
			el--;
			return const_iterator(temp);
		}

		const T* operator->(){
			return el;
		}
		const T& operator *()const {
			return *el;
		}
	
		const_iterator& operator= (const_iterator another){
			el = another.el;
			return *this;
		}

		bool operator== (const_iterator& i){
			return (el == i.el ? true:false);
		}

		bool operator!= (const_iterator& i){
			return !(*this == i);
		}

		T* get(){
			return el;
		}

		int operator-(const_iterator tp){
			T* t=el;
			int i=0;
			for(; t!=tp.get(); t++, i++);
			return i;
		}

		const_iterator operator+(int n){
			for (int i=0; i<n; ++i)
				++*this;
			return *this;
		}

	private:
		T* el;
	};
private:
	int position(iterator pos){
		T* tp=arr;
		unsigned int i;
		for(i=0; tp!=pos.get(); ++tp, ++i);
		return i;
	}
public:


	TVector(){
		tsize=0;
		tcapacity=10;
		arr=new T [tcapacity];
	};


	explicit TVector(unsigned int f_cap){
		tsize=0;
		tcapacity=f_cap;
		arr=new T [tcapacity];
	};

	TVector(int n, T value){
		tsize=n;
		tcapacity=n;
		arr=new T [tcapacity];
		for (int i = 0;  i < n; ++i)
			arr[i] = value;
	}

	TVector ( iterator  beg, iterator  end){
		tsize = 0;
		tcapacity = 10;
		arr = new T[tcapacity];
		for (;beg!= end; ++beg)
			push_back(*beg);
	}

	~TVector(){
		delete[] arr;
	}

	void push_back(T n){
		if (tsize==tcapacity){
			T* tptr=new T[tcapacity*=2];
			for(unsigned int i=0; i<tsize; i++)
				tptr[i]=arr[i];
			delete [] arr;
			arr=tptr;
		}
		arr[tsize]=n;
		tsize++;
	}

	iterator begin(){
		iterator pos_tp (&arr[0]);
		return pos_tp;
	}

	iterator end(){
		iterator pos_tp (&arr[tsize]);
		return 	pos_tp;
	}

	const_iterator begin() const {
		const_iterator pos_tp (&arr[0]);
		return pos_tp;
	}

	const_iterator end() const {
		const_iterator pos_tp (&arr[tsize]);
		return 	pos_tp;
	}

	T pop_back(){
		return arr[--tsize];
	};

	void clear(){
		tsize=0;
	};

	void swap(TVector& temp){
		std::swap(arr, temp.arr);
		std::swap(tcapacity, temp.tcapacity);
		std::swap(tsize, temp.tsize);
	}

	void reserve (unsigned int ncap){
		if (ncap<=tsize)
			return; 
		T* tp=new T [ncap];
		for(unsigned int i=0; i<tsize; i++)
			tp[i]=arr[i];
		delete[] arr;
		arr=tp;
		tcapacity=ncap;
	}

	T& operator[](unsigned int n){ //как сделать так, чтобы записывать?
		return arr[n];
	}

	const T& operator[](unsigned int n) const { //как сделать так, чтобы записывать?
		return arr[n];
	};

	TVector& operator=(const TVector& temp){
		if(temp.size()> tcapacity){
			while(temp.size()> tcapacity)
				tcapacity*=2;
			delete [] arr;
			arr=new T [tcapacity];
		}
		for (unsigned int i=0; i<temp.size(); i++)
			arr[i]=temp[i];
		tsize=temp.size();
		return *this;
	};

	unsigned int size(){
		return tsize;
	};

	unsigned int size() const {
		return tsize;
	};

	unsigned int capacity(){
		return tcapacity;
	};

	unsigned int capacity() const {
		return tcapacity;
	};

	bool empty (){
		if (tsize==0)
			return true;
		return false;
	};

	bool empty ()const {
		if (tsize==0)
			return true;
		return false;
	};

	T& back(){
		return arr[tsize-1];
	};

	const T& back() const {
		return arr[tsize-1];
	};

	T& front(){
		return arr[0];
	};

	const T& front() const {
		return arr[0];
	};

	void resize(unsigned int n){
		if(n==tsize)
			return;
		else if (n<tsize){
			unsigned int i=tsize;
			for (;i>n; i--){
				tsize--;	
			}
		}
		else if (n>tsize)
			if(n>tcapacity){
				while (n>tcapacity){
					tcapacity*=2;
				}
				T* tptr=new T[tcapacity];
				for(unsigned int i=0; i<tsize; i++)
					tptr[i]=arr[i];
				delete[] arr;
				arr=tptr;
			}
		tsize=n;
	}

	void resize(unsigned int n, T val){
		if(n==tsize)
			return;
		else if (n<tsize){
			unsigned int i=tsize;
			for (;i>n; i--){
				tsize--;	
			}
		}
		else if (n>tsize)
			if(n>tcapacity){
				while (n>tcapacity){
					tcapacity*=2;
				}
				T* tptr=new T[tcapacity];
				for(unsigned int i=0; i<tsize; i++)
					tptr[i]=arr[i];
				for (unsigned int i=tsize; i<n; i++)
					tptr[i]=val;
				delete[] arr;
				arr=tptr;
			}
		tsize=n;
	}

	iterator erase(iterator pos){ // ак это сделать красивее? ¬ычитать!
		T* tp=arr;
		unsigned int i;
		for(i=0; tp!=pos.get(); ++tp, ++i);
		tsize-=1;
		++pos;
		for(unsigned int k=i;k<tsize; k++){
			arr[k]=arr[k+1];
		}
		return pos;
	}

	iterator erase(iterator beg, iterator end){ 
		T* tp=arr;
		int i;
		for(i=0; tp!=beg.get(); ++tp, ++i);
		int k;
		for(k=0; tp!=end.get(); ++tp, ++k);
		unsigned int p = 0;
		for(unsigned int j=i; (k+p-1)<tsize; j++){
			arr[j]=arr[k+p-1];
			++p;
		}
		tsize=tsize-k+i+1;
		++end;
		return end;
	}

	iterator insert(iterator it, int n, T val){
		insert(position(it), n, val);
		for (int i=0; i<n; ++i)
			--it;
		return it;
	}

	void insert(int p, int n, T val){
		if((tsize+n)>=tcapacity)
			reserve(tsize+n);
		tsize+=n;
		for(int i=tsize-1; i>=(p+n); i--)
			arr[i]=arr[i-n];
		for(int i=p; i<p+n; ++i)
			arr[i]=val;
	}

	iterator insert(iterator pos, iterator beg, iterator end){ 
		int p=position(pos);
		--end;
		for (; beg!=end; --end,--pos)
			insert(p, 1, *end);
		insert(p,1,*beg);
		return --pos;
	}

	iterator insert(iterator pos, T* beg, T* end){
		int p=position(pos);
		--end;
		for (; beg!=end; --end, --pos)
			insert(p, 1, *end);
		insert(p,1,*beg);
		return --pos;
	}

private:
	unsigned int tsize;
	unsigned int tcapacity;
	T* arr;
};


void show(const TVector<TFoo>& a){
		TVector<TFoo>:: const_iterator pos_tp; 
		pos_tp = a.begin();
		TVector<TFoo>:: const_iterator end_tp;
		end_tp = a.end();
		for (int i=0; pos_tp!= end_tp; ++pos_tp, ++i)
			cout<< "a["<<i<<"]="<<(*pos_tp).Value<<endl;
		cout<<"-----------------------------------------"<< endl;
}


int main(){
	{
	TVector<TFoo>a,b;
	TFoo temp;
	for (int i=0; i<10; i++){
		temp.Value = i;
		a.push_back(temp);
	}
	for (int i=9; i>=0; i--){
		temp.Value = i;
		b.push_back(temp);
	}
	TVector<TFoo>::iterator pos;
	TFoo ab;
	for(int i=9; i>=6; i--)
		ab=a.pop_back();
	cout << a.size()<<endl;
	a.clear();
	cout<< a.empty()<< endl;
	show(a);
	a.swap(b);
	show(a);
	show(b);
	a.resize(8);
	show(a);
	a.reserve(5);
	show(a);
	a.reserve(25);
	show(a);
	temp.Value = 100;
	a.insert(a.begin()+7, 5, temp);
	show(a);
	a.erase(a.begin(), a.begin()+3);
	show(a);
	}
	TFoo::PrintStats();
	return 0;
}