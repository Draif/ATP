#include <iostream>
#include <time.h>
#include "my_time.h"
using namespace std;



class TestClass {
public:
	
	void test1 (){
		TDateTime a;
		cout << "Time now:\n" << a << endl<< "Another test using static method:" << endl
			<< a.Now() << endl;
		//string s;
		//cout << "DateTiem should be in format : hh:mm:ss day.month.year. Please, enter the date.\n";
		//getline(cin, s);
		//TDateTime b (s);
		//cout << b;
	}
	
	void test2() {
		int year = 2016;
		int month = 6;
		int day = 29;
		int hour = 12;
		int minute = 0;
		int second = 0;
		TDateTime a (year, month, day, hour, minute, second);
		cout << "Time on 29th of June 2016 at midday:\n" << a << endl;
	}
	
	void test3() {
		TDateTime b;
		int year = 2020;
		int month = 6;
		int day = 29;
		TDateTime a (year, month, day);
		b = a;
		cout << "Time on 29th of June 2020:\n" << b << endl;
	}

	void test4 () {
		TDateTime a;
		time_t timer = a.GetUnixTimestamp();
		cout << "Seconds since 1st January 1970" << endl << timer << endl;
		TDateTime b (timer);
		cout << "Time now:\n" << b << endl;
	}

	void test5 () {
		cout << "This test will show separate parts of date" << endl;
		TDateTime a;
		cout << "Year: " << a.Year() << endl;
		cout << "Month: " << a.Month() << endl;
		cout << "Day: " << a.Day() << endl;
		cout << "Hour: " << a.Hour() << endl;
		cout << "Minute: " << a.Minute() << endl;
		cout << "Second: " << a.Second() << endl;
		cout << "Day of week: " << a.DayOfWeek() << endl << endl;
	}

	void TestTTimeSpan () {
		TTimeSpan a;
		cin >> a;
		cout << a << endl;
		cout << a.GetUnixTimestamp();
	}

	void test6 () {
		cout << "Test for 'TDateTime operator+ (const TTimeSpan& delta);'" << endl << "______________________________________________\n";
		TTimeSpan test_span;
		cin >> test_span;
		TDateTime date;
		cout << "Time now: \n" << date << endl;
		cout << "After "<< endl << test_span << endl << "will be" << endl << date + test_span << endl;
	}

	void test7 () {
		cout << "Test for 'TDateTime operator- (const TTimeSpan& delta);'" << endl << "______________________________________________\n";
		TTimeSpan test_span;
		cin >> test_span;
		TDateTime date;
		cout << "Time now: \n" << date << endl;
		cout << "This time:"<< endl << test_span << endl << " ago was" << endl << date - test_span << endl;

	}

	void test8 () {
		cout << "Test for '	TTimeSpan operator- (const TDateTime& delta);'\nFirst date should be more than second" << endl << "______________________________________________\n";
		TDateTime a;
		TDateTime b;
		cin >> a >> b;
		if (!(a < b) )
			cout << a-b;
		else 
			cout << "First Date is less than Second Date" << endl;
	}

	void test9 () {
		cout << "Test for '	TTimeSpan operator- (const TDateTime& delta);'\nFirst date should be more than second" << endl << "______________________________________________\n";
		TDateTime a;
		TTimeSpan t1 (1,0,0);
		TTimeSpan t2 (1,0,0);
		TTimeSpan t4 (1,0,0);
		TTimeSpan t5 (1,0,0);
		TTimeSpan t3 (4,0,0);
		cout << a+t1+t2+ t4 + t5 << endl;
		cout << a +t3;
	}
};


int main(){
	TDateTime dt = TDateTime::Now();
	cout << dt << endl << endl;
	TTimeSpan ts(1, 1, 1);
	cout << (dt + ts) << endl << endl;
	dt += ts;
	dt = dt + ts;
	cout << dt << endl << endl;
	dt -= ts;
	cout << dt << endl << endl;
	TestClass test;
	test.test1();
	test.test2();
	test.test3();
	test.test4();
	test.test5();
	//test.TestTTimeSpan();
	test.test6();
	test.test7();
	test.test8();
	test.test9();
	return 0;
}