# define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include "my_time.h"
#include <string>
using namespace std;
int time_zone = 4;

	TTime& TTime:: operator= (struct tm* equal){
		year = equal->tm_year + 1900;
		month = equal->tm_mon + 1;
		day = equal->tm_mday;
		hour = equal->tm_hour;
		minute = equal->tm_min;
		second = equal->tm_sec;
		wday = equal->tm_wday;
		return *this;
	}

	void TTimeSpan :: ToCalendar() {
		struct tm* temp;
		temp = localtime (&span_time);
		temp->tm_year = temp->tm_year;
		temp->tm_mday = temp->tm_mday;
		temp->tm_hour = temp->tm_hour;
		span = temp;
	}

	TTimeSpan:: TTimeSpan (){
	}
	TTimeSpan :: TTimeSpan (const TTimeSpan& another){
		*this = another;
	}
	TTimeSpan:: TTimeSpan (time_t period){
		span_time = period;
		ToCalendar();
	}
	TTimeSpan:: TTimeSpan (int year, int month, int day, int h, int m, int s){
		span.year = year;
		span.month = month;
		span.day = day;
		span.hour = h;
		span.minute = m;
		span.second = s;
	}


	TTimeSpan:: TTimeSpan (int year, int month, int day){
		span.year = year;
		span.month = month;
		span.day = day;
	}

	void TTimeSpan:: ChangeYear (int year){
		span.year = year;
	}
	void TTimeSpan:: ChangeMonth (int month){
		span.month = month;
	}
	void TTimeSpan:: ChangeDay (int day){
		span.day = day;
	}
	void TTimeSpan:: ChangeHour (int hour){
		span.hour = hour;
	}
	void TTimeSpan:: ChangeMinute (int min){
		span.minute = min;
	}
	void TTimeSpan:: ChangeSecond (int second){
		span.second = second;
	}
	int TTimeSpan:: Year () const{
		return span.year;
	}
	int TTimeSpan:: Month () const{
		return span.month;
	}
	int TTimeSpan:: Day () const{
		return span.day;
	}
	int TTimeSpan:: Hour () const{
		return span.hour;
	}
	int TTimeSpan:: Minute () const{
		return span.minute;
	}
	int TTimeSpan:: Second () const{
		return span.second;
	}
	TTimeSpan& TTimeSpan:: operator= (const TTimeSpan& equal){
		span.year = equal.Year();
		span.month = equal.Month();
		span.day = equal.Day();
		span.hour = equal.Hour();
		span.minute = equal.Minute();
		span.second = equal.Second();
		span_time = equal.GetUnixTimestamp();
		return *this;
	}

	time_t TTimeSpan :: GetUnixTimestamp() const{
		return span_time;
	}
	bool TTimeSpan:: operator< (TTimeSpan& compare){
		return (span.year < compare.Year() ||  span.month < compare.Month() || span.day < compare.Day() || span.hour < compare.Hour() ||
			span.minute < compare.Minute() || span.second < compare.Second());
	}

	TDateTime :: TDateTime (){
		time_t timer;
		time( &timer );
		current_time = timer;
		ToCalendar();
	}
	void TDateTime :: ToCalendar() {
		struct tm* temp;
		temp = localtime (&current_time);
		timeinfo = temp;
	}

	TDateTime :: TDateTime (const TDateTime& temp){
		timeinfo.year = temp.Year();
		timeinfo.month = temp.Month();
		timeinfo.day = temp.Day();
		timeinfo.hour = temp.Hour();
		timeinfo.minute = temp.Minute();
		timeinfo.second = temp.Second();
		timeinfo.wday = temp.DayOfWeek();
		current_time = temp.GetUnixTimestamp();
	}

	TDateTime :: TDateTime (time_t t){
		current_time = t;
		ToCalendar();
	}

	TDateTime :: TDateTime (int year, int month, int day, int h, int m, int s){
		struct tm* temp;
		time_t timer;
		time (&timer);
		temp = localtime (&timer);
		temp->tm_year = year - 1900;
		temp->tm_mon = month - 1;
		temp->tm_mday = day;
		temp->tm_hour = h;
		temp->tm_min = m;
		temp->tm_sec = s;
		current_time = mktime (temp);
		ToCalendar();
	}

	TDateTime :: TDateTime (int year, int month, int day){
		struct tm* temp;
		time_t timer;
		time (&timer);
		temp = localtime (&timer);
		temp->tm_year = year - 1900;
		temp->tm_mon = month - 1;
		temp->tm_mday = day;
		temp->tm_hour = 0;
		temp->tm_min = 0;
		temp->tm_sec = 0;
		current_time = mktime (temp);
		ToCalendar();
	}

	TDateTime :: TDateTime (string s){
		// DateTiem should be in format : hh:mm:ss day.month.year
		int sec, min, hour, day, month, year;
		int zero = (int)'0';
		hour = ((int)s[0] - zero)* 10 + (int)s[1] - zero;
		min = ((int)s[3] - zero)* 10 + (int)s[4] - zero;
		sec = ((int)s[6] - zero)* 10 + (int)s[7] - zero;
		day = ((int)s[9] - zero)* 10 + (int)s[10] - zero;
		month = ((int)s[12] - zero)* 10 + (int)s[13] - zero;
		year = 	((int)s[15] - zero)* 1000 + ((int)s[16] - zero) * 100 + ((int)s[17] -zero)* 10 + (int)s[18] - zero;
		struct tm* temp;
		time_t timer;
		time (&timer);
		temp = localtime (&timer);
		temp->tm_year = year - 1900;
		temp->tm_mon = month - 1;
		temp->tm_mday = day;
		temp->tm_hour = hour;
		temp->tm_min = min;
		temp->tm_sec = sec;
		current_time = mktime (temp);
		ToCalendar();
	}

	int TDateTime :: Year () const {
		return timeinfo.year;
	}

	int TDateTime :: Month () const {
		return timeinfo.month;
	}

	int TDateTime :: Day () const {
		return timeinfo.day;
	}

	int TDateTime :: Hour () const {
		return timeinfo.hour;
	}

	int TDateTime :: Minute () const{
		return timeinfo.minute;
	}

	int TDateTime :: Second () const{
		return timeinfo.second;
	}

	int TDateTime :: DayOfWeek () const{
		return timeinfo.wday;
	}

	time_t TDateTime :: GetUnixTimestamp() const{
		return current_time;
	}
	
	TDateTime TDateTime :: Now (){
		TDateTime a;
		return a;
	}

	bool TDateTime :: operator< (TDateTime& a){
		return ( current_time < a.GetUnixTimestamp() );
	}

	TDateTime& TDateTime :: operator= (const TDateTime& temp){
		timeinfo.year = temp.Year();
		timeinfo.month = temp.Month();
		timeinfo.day = temp.Day();
		timeinfo.hour = temp.Hour();
		timeinfo.minute = temp.Minute();
		timeinfo.second = temp.Second();
		timeinfo.wday = temp.DayOfWeek();
		current_time = temp.GetUnixTimestamp();
		return *this;
	}

	TDateTime TDateTime :: operator+ (const TTimeSpan& delta){
		struct tm* temp;
		time_t timer;
		time_t current_time;
		time (&timer);
		temp = localtime (&timer);
		temp->tm_year = timeinfo.year + delta.Year() - 1900;
		temp->tm_mon = timeinfo.month + delta.Month() - 1;
		temp->tm_mday = timeinfo.day + delta.Day();
		temp->tm_hour = timeinfo.hour + delta.Hour();
		temp->tm_min = timeinfo.minute + delta.Minute();
		temp->tm_sec = timeinfo.second + delta.Second();
		current_time = mktime (temp);
		return TDateTime( current_time);
	}


	TDateTime TDateTime :: operator- (const TTimeSpan& delta){
	struct tm* temp;
		time_t timer;
		time_t current_time;
		time (&timer);
		temp = localtime (&timer);
		temp->tm_year = timeinfo.year - delta.Year() - 1900;
		temp->tm_mon = timeinfo.month - delta.Month() - 1;
		temp->tm_mday = timeinfo.day - delta.Day();
		temp->tm_hour = timeinfo.hour - delta.Hour();
		temp->tm_min = timeinfo.minute - delta.Minute();
		temp->tm_sec = timeinfo.second - delta.Second();
		current_time = mktime (temp);
		return TDateTime( current_time);
	}

	TTimeSpan TDateTime :: operator- (const TDateTime& delta){
		return TTimeSpan (timeinfo.year - delta.Year(), timeinfo.month - delta.Month(), timeinfo.day - delta.Day(), timeinfo.hour - delta.Hour(), timeinfo.minute - delta.Minute(), timeinfo.second - delta.Second());
	}

	TDateTime TDateTime:: operator-= (const TTimeSpan& delta){
		*this = *this - delta;
		return *this;
	}

	TDateTime TDateTime:: operator+= (const TTimeSpan& delta){
		*this = *this + delta;
		return *this;
	}


	ostream& operator<< (ostream& out, const TDateTime& date){
		out << date.Hour()<< ":"
			<< date.Minute()  << ":"
			<< date.Second()  << " "
			<< date.Day() << "." 
			<< date.Month() << "." 
			<< date.Year()<< endl;
		return out;
	}

	istream& operator>> (istream& in, TDateTime& date){
		int year, month, day, hour, min, sec;
		cout << "Please enter date and time:\n";
		cout << "Year: ";
		in >> year;
		cout << "Month: ";
		in >> month;
		cout << "Day: ";
		in >> day;
		cout << "Hour: ";
		in >> hour;
		cout << "Minute: ";
		in >> min;
		cout << "Second: ";
		in >> sec;
		TDateTime temp (year, month, day, hour, min, sec);
		date = temp;
		return in;
	}


	ostream& operator<< (ostream& out, const TTimeSpan& date){
		out << date.Hour()<< ":"
			<< date.Minute()  << ":"
			<< date.Second()  << " "
			<< date.Day() << "." 
			<< date.Month() << "." 
			<< date.Year()<< endl;
		return out;
	}

	istream& operator>> (istream& in, TTimeSpan& date){
		int year, month, day, hour, min, sec;
		cout << "Please enter time span:\n";
		cout << "Year: ";
		in >> year;
		cout << "Month: ";
		in >> month;
		cout << "Day: ";
		in >> day;
		cout << "Hour: ";
		in >> hour;
		cout << "Minute: ";
		in >> min;
		cout << "Second: ";
		in >> sec;
		TTimeSpan temp (year, month, day, hour, min, sec);
		date = temp;
		return in;
	}
