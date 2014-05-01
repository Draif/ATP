#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <stdlib.h>
#include "my_set.h"


using namespace std;


struct TFoo {
    int Value;
    TFoo() : Value(0) { ++Created; }
    explicit TFoo(int value) : Value(value) { ++Created; }
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


typedef TRBTree<TFoo>::iterator RB_Iterator;

static void Test3 (){
	TRBTree<TFoo> tree1;
	TRBTree<TFoo> tree2;
	RB_Iterator found;
	int key = 0;

	cout << "First tree inserted with:" << endl;
	for (int i = 0; i < 10; ++i){
		TFoo a;
		a.Value = rand()%100;
		cout << a.Value << endl;
		if ( i == 5)
			key = a.Value;
		tree1.insert (a);
	}

	TFoo a;
	a.Value = key;

	cout << "First tree now:" << endl;
	for (RB_Iterator pos = tree1.begin(); pos != tree1.end(); ++pos)
		cout << (*pos).Value << endl;

	cout << "Try to find: "<< a.Value<< endl;
	found = tree1.find(a);
	cout << "Found: " << (*found).Value << endl;

	cout << "Try to erase: " << (*found).Value << endl;
	tree1.erase ( found );
	cout << "First tree now:" << endl;
	for (RB_Iterator pos = tree1.begin(); pos != tree1.end(); ++pos)
		cout << (*pos).Value << endl;

	cout << "Second tree inserted with:" << endl;
	for (int i = 0; i < 10; ++i){
		TFoo a;
		a.Value = rand()%100;
		cout << a.Value << endl;
		tree2.insert (a);
	}
	cout << "Second tree now:" << endl;
	for (RB_Iterator pos = tree2.begin(); pos != tree2.end(); ++pos)
		cout << (*pos).Value << endl;

	cout << "Swap trees" << endl;
	tree1.swap(tree2);
	cout << "First tree now:" << endl;
	for (RB_Iterator pos = tree1.begin(); pos != tree1.end(); ++pos)
		cout << (*pos).Value << endl;
	cout << "Second tree now:" << endl;
	for (RB_Iterator pos = tree2.begin(); pos != tree2.end(); ++pos)
		cout << (*pos).Value << endl;

	cout << "Checking operator= "<< endl;
	tree1 = tree2;
	cout << "First tree now:" << endl;
	for (RB_Iterator pos = tree1.begin(); pos != tree1.end(); ++pos)
		cout << (*pos).Value << endl;
}


static void Test4() {
    typedef TRBTree<TFoo> TTreeFoo;

    TTreeFoo a;
    for (int i = 0; i < 100; ++i)
        a.insert(TFoo(i));

    for (int i = 0; i < 100; ++i)
        TTreeFoo::iterator iter = a.find(TFoo(i));

    TTreeFoo b = a;

    a.swap(b);
}

int main() {
	Test3();
    Test4();
    TFoo::PrintStats();
    return 0;
}