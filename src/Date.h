#pragma once
#include<string>
#include<Arduino.h>
#include<RTClib.h>
using namespace std;
struct Date {
	unsigned int year;
	int month;
	int day;
};
Date NewDate(int year,int month,int day);
int GetWeek(Date date);
bool IsLeapYear(int year);
Date GetFirstWeek(Date date, int week);
Date GetLastWeek(Date date, int week);
int GetMonthDays(Date date);
int GetMonthWeeks(Date date, int week);
int GetMonthWeek(Date date, int week, int index);
bool Judge(Date A, Date B);
int ACDays(Date date);
int DaysBetween(Date A, Date B);
Date AddDays(Date date, int day);
String PrintDate(Date date);
bool Check(Date date);
Date Trans(String str);
Date GetLocalDate(DateTime dateTime);
String GetLocalTime(DateTime dateTime);