#include <iostream>
#include <vector>
using namespace std;


struct TFoo {
    int Value;
    explicit TFoo(int value) : Value(value) { ++Created; }
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
class TList{
	struct node{
		friend class TList;
		node(T& a): val(a){
		}
		node(){
			prev=0;
			next=0;
		}

		~node(){
		}
		T val;
		node* next;
		node* prev;
		bool operator== (node& another){
			return (val == another.val && next == another.next && prev == another.prev);
		}
		bool operator!= (node& another){
			return !(*this == another);
		}
	};
	node* first;
	node* last;
	node* limit;
public:
	class iterator{
		friend class TList;
	public:
		iterator(node *t):el(t){}
		iterator():el(0){
		};

		iterator& operator++(){
			el=el->next;
			return *this;
		};
		
		iterator& operator++(int){
			node* temp;
			temp = el;
			el=el->next;
			return iterator(temp);
		};

		iterator& operator --(){
			el=el->prev;
			return *this;
		}

		iterator& operator--(int){
			node* temp = el;
			el=el->prev;
			return  iterator(temp);
		}

		T& operator *(){
			return el->val;
		}

		T* operator->()const{
			return &(el->val);
		}

		bool operator == (iterator& i){
			return (el == i.el ? true:false);
		}

		bool operator != (iterator& i){
			return !(*this == i);
		}

		iterator& operator=(iterator& tp){
			el=tp.get();
			return *this;
		}

		node* get(){
			return el;
		}

	private:
		node* el;
	};

	class const_iterator{
		friend class TList;
	public:
		const_iterator(node *t):el(t){}
		const_iterator():el(0){
		};

		const_iterator& operator++(){
			el=el->next;
			return *this;
		};
		const_iterator& operator++(int){
			node* temp;
			temp = el;
			el=el->next;
			return const_iterator(temp);
		};
		const_iterator& operator --(){
			el=el->prev;
			return *this;
		}
		
		const_iterator& operator --(int){
			node* temp = el;
			el=el->prev;
			return const_iterator(temp);
		}

		const T& operator *() const{
			return el->val;
		}

		const T* operator->()const{
			return &(el->val);
		}

		bool operator == (const_iterator& i){
			return (el == i.el ? true:false);
		}

		bool operator != (const_iterator& i){
			return !(*this == i);
		}

		const_iterator& operator=(const_iterator& tp){
			el=tp.get();
			return *this;
		}

		node* get(){
			return el;
		}


	private:
		node* el;
	};
private:
	void Insert(node* where, T val){
		node* n=new node;
		n->val=val;
		Insert(where,n);
	};

	void Insert(node* where, node* tnode){
		if (where==first){
			push_front(tnode->val);
			delete tnode;
		}
		else {
			tnode->prev=where->prev;
			tnode->next=where;
			where->prev->next=tnode;
			where->prev=tnode;
		}	
	};	

	void Insert(node* where, TList &other){
		if (where->prev!=0){
			(where->prev)->next=other.first;
			other.first->prev=where->prev;
			where->prev=other.last;
			other.last->next=where;
			other.first=0;
			other.last=0;
		}
		else{
			first=other.first;
			where->prev=other.last;
			other.last->next=where;
			other.first=0;
			other.last=0;
		}
	};

	void Extract(node* tnode){
		if(tnode==first && tnode==last){
			first=0;
			last=0;
			delete tnode;
			return;
		}
		if(tnode==first)
			first=tnode->next;
		else
			tnode->prev->next=tnode->next;
		if(tnode==last)
			last=tnode->prev;
		else
			tnode->next->prev=tnode->prev;
		delete tnode;
		return;
	};

	void Extract(node* tnode, iterator beg){
		if(tnode==first && tnode==last){
			first=0;
			last=0;
			delete tnode;
			return;
		}
		++beg;
		if(tnode==first)
			first=tnode->next;
		else
			tnode->prev->next=tnode->next;
		if(tnode==last)
			last=tnode->next;
		else
			tnode->next->prev=tnode->prev;
		delete tnode;
		return;
	};


public:
	TList(){
		first=last=0;
		limit = new node;
		limit->next = 0;
		limit-> prev = 0;
	}

	TList ( TList<T>& another){
		limit = new node;
		limit->next = 0;
		limit-> prev = 0;
		first = last = 0;
		*this = another;
	}

	TList (T val, int n){
		first=last=0;
		limit = new node;
		limit->next = 0;
		limit-> prev = 0;
		for(int i=0; i<n; ++i)
			push_back(val);
	}

	~TList(){
		clear();
		delete limit;
	}

	iterator begin(){
		return iterator(first);
	}

	iterator end (){
		return iterator(limit);
	}

	const_iterator begin() const {
		return const_iterator(first);
	}

	const_iterator end() const {
		return const_iterator(limit);
	}

	bool empty () const {
		return first==0? true:false;
	}

	void clear (){
		while( first != 0 && first != limit ){
			last=first->next;
			delete first;
			first=last;
		}
		first=last=0;
	}

	void push_back (T val) {
		node* tp=new node;
		tp->val=val;
		tp->next=limit;
		if(first==0){
			tp->prev=limit;
			limit->prev = tp;
			first=last=tp;	
		}
		else {
			tp->prev=last;
			last->next=tp;
			last=tp;
			limit->prev = tp;
		}
	}

	void push_front(T val){
		node* tp=new node;
		tp->val=val;
		tp->prev=limit;
		if (last==0)
		{last=first=tp;
		tp->next=limit;
		}
		else{
			tp->next=first;
			first->prev=tp;
			first=tp;
		}
		return;
	}

	T pop_back(){
		T tp=last->val;
		last=last->prev;
		delete last->next;
		last->next=limit;
		limit->prev = last;
		return tp;
	}

	T pop_front () {
		T tp=first->val;
		first=first->next;
		delete first->prev;
		first->prev=limit;
		return tp;
	}

	T& front (){
		return first->val;
	}

	T& back () {
		return last->val;
	}

	const T& front ()const{
		return first->val;
	}

	const T& back () const {
		return last->val;
	}

	TList& operator=( TList &other){
		if(this!=&other){
			clear();
			first=0;
			last=0;
			node* temp;
			temp=other.first;
			while(temp!=other.last){
				push_back(temp->val);
				temp=temp->next;
			}
			push_back(temp->val);
		}
		return *this;
	};

	void swap(TList& temp){
		std::swap(first, temp.first);
		std::swap(last, temp.last);
		std::swap(limit, temp.limit);
		return;
	}
	


	void insert(iterator pos, T val){
		Insert(pos.get(),val);
		return;
	}

	void insert (iterator pos, int n, T val){
		for (int i=0; i<n; i++)
			Insert(pos.get(), val);
		return;
	}

	void insert (iterator pos, iterator beg, iterator end){
		--end;
		for (; beg!=end; --end, --pos)
			Insert(pos.get(), *end);
		Insert(pos.get(), *end);
		return;
	}

	
	iterator erase(iterator pos){
		Extract(pos.get(), pos);
		return pos;
	}

	iterator erase (iterator beg, iterator end){ 
		for (;beg!=end;){
			iterator tp=beg;
			++beg;
			Extract(tp.get());
		}
		return end;
	}

	void splice (iterator pos, TList& lt){
		if ( empty()){
			iterator beg=lt.begin();
			iterator tp=lt.end();
			for (; beg!=tp; ++beg){
				push_back(*beg);
			}
		}
		else {
			iterator tp=lt.end();
			iterator beg=lt.begin();
			--tp;
			for (; tp!=beg; --pos){
				insert(pos,*tp);
				iterator t=tp;
				--tp;
				lt.erase(t);
			}
			insert(pos,*tp);
			lt.erase(tp);
			return;
		}
	}

	void splice (iterator pos, TList& lt, iterator it){
		insert(pos,*it);
		lt.erase(it);
		return;
	}
	
	void splice (iterator pos, TList& lt, iterator first, iterator last){
		iterator tp=last;
		--tp;
		for (; tp!=first; --pos){
			insert(pos,*tp);
			iterator t=tp;
			--tp;
			lt.erase(t);
		}
		insert(pos,*tp);
		lt.erase(tp);
		return;
	}
};



void show (const TList<TFoo>& a){
		TList<TFoo> :: const_iterator pos_tp = a.begin();
		TList<TFoo> :: const_iterator end_tp = a.end();
		if (!a.empty())
			for (int i=1; pos_tp != end_tp; ++pos_tp, i++ )
				cout << "Element "<<i<< " : "<< pos_tp->Value <<endl;
		cout <<"________________________________________________"<<endl;

	}

int main(){
	{TList <TFoo> a,b;
	TList<TFoo> aa;
	TList<TFoo>  bb;
	show(bb);
	for (int i=0; i<10; i++){
		TFoo temp;
		temp.Value = i;
		a.push_back(temp);
	}
	for (int i=0; i<10; i++){
		TFoo temp;
		temp.Value = i;
		b.push_front(temp);
	}
	if (a.empty())
	cout <<"true\n";
	else 
	cout << "false\n";
	b=a;
	show(b);
	a.swap(b);
	show(a);
	show(b);
	TFoo p;
	show(b);
	for (int i=0; i<5; i++)
		p=a.pop_front();
	show(a);
	a.clear();
	if (a.empty())
	cout <<"true\n";
	else 
	cout << "false\n";
	show(a);
	TFoo temp;
	temp.Value = 20;
	a.insert(a.begin(), temp);
	show(a);
	a.insert(a.begin(), 2, temp);
	show(a);
	a.insert(a.begin(), b.begin(), b.end());
	show(a);
	show(a);
	a.erase(a.begin());
	show(a);
	a.erase(a.begin(), a.end());
	show(a);
	a.splice(a.begin(), b);
	show(a);
	b.splice(b.begin(), a, a.begin());
	show(b);
	show(a);
	b.splice(b.begin(), a, a.begin(), a.end());
	show(b);
	show(a);

        {
            typedef TList<TFoo> TListFoo;
            TListFoo a;
            for (int i = 0; i < 10; ++i)
                a.push_back(TFoo(i));
			show(a);
            TListFoo b = a;
            for (int i = 0; i < 10; ++i)
                b.push_front(TFoo(i));
			show(b);
            a.swap(b);
			show(a);
			show(b);
        }
        }
	TFoo :: PrintStats();
	return 0;

}
