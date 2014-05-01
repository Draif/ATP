#pragma once
#include <string>
#include <iostream>
#include <string>


struct TTime{
	int year;
	int month;
	int day; 
	int hour;
	int minute;
	int second;
	int wday;
	TTime(){
		wday = year = month = day = hour = minute = second = 0;
	}
	TTime& operator= (struct tm* equal);
};


class TTimeSpan{
	TTime span;
	time_t span_time;
	void ToCalendar();
public:
	TTimeSpan ();
	TTimeSpan (const TTimeSpan&);
	TTimeSpan (int year, int month, int day, int h, int m, int s);
	TTimeSpan (int year, int month, int day);
	TTimeSpan (time_t);
	void ChangeYear (int year);
	void ChangeMonth (int month);
	void ChangeDay (int day);
	void ChangeHour (int hour);
	void ChangeMinute (int min);
	void ChangeSecond (int second);
	int Year () const;
	int Month () const;
	int Day () const;
	int Hour () const;
	int Minute () const;
	int Second () const;
	time_t GetUnixTimestamp() const;
	TTimeSpan& operator= (const TTimeSpan& equal);
	bool operator< (TTimeSpan& compare);
	//bool operator< (TDateTime& compare);
};

class TDateTime{
	TTime timeinfo;
	time_t current_time;
	void ToCalendar ();
public:
	TDateTime ();
	TDateTime (const TDateTime&);
	TDateTime (time_t t);
	TDateTime (int year, int month, int day, int h, int m, int s);
	TDateTime (int year, int month, int day);
	TDateTime (std::string s);
	int Year () const;
	int Month () const;
	int Day () const;
	int Hour () const;
	int Minute () const;
	int Second () const;
	int DayOfWeek () const;
	time_t GetUnixTimestamp() const;
	static TDateTime Now ();
	bool operator< (TDateTime& a);
	TDateTime& operator= (const TDateTime&);
	TDateTime operator+ (const TTimeSpan& delta);
	TDateTime operator- (const TTimeSpan& delta);
	TTimeSpan operator- (const TDateTime& delta);
	TDateTime operator-= (const TTimeSpan& delta);
	TDateTime operator+= (const TTimeSpan& delta);
};

std::ostream& operator<<(std::ostream& out, const TDateTime& date);
std::istream& operator>> (std::istream& in, TDateTime& date);
std::ostream& operator<<(std::ostream& out, const TTimeSpan& date);
std::istream& operator>> (std::istream& in, TTimeSpan& date);