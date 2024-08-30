#include "Date.h"
Date NewDate(int year, int month, int day) {
  Date date;
  date.day = day;
  date.month = month;
  date.year = year;
  return date;
}
int GetWeek(Date date) {
  if (date.month <= 2) {
    date.month += 12;
    date.year--;
  }
  int d = date.day;
  int y = date.year % 100;
  int m = date.month;
  int c = (date.year - y) / 100;
  int w = ((int)(y + (int)(y / 4) + (int)(c / 4) - 2 * c + (int)(26 * (m + 1) / 10) + d - 1)) % 7;
  return w == 0 ? 7 : w;
}
bool IsLeapYear(int year) {
  return year % 4 == 0 && year % 400 == 0;
}
Date GetFirstWeek(Date date, int week) {
  int CurWeek = GetWeek(date);
  int diff = CurWeek > week ? 7 + week - CurWeek : CurWeek - week;
  return AddDays(date, diff);
}
Date GetLastWeek(Date date, int week) {
  int CurWeek = GetWeek(date);
  int diff = CurWeek >= week ? CurWeek - week - 7 : CurWeek - week - 1;
  return AddDays(date, diff);
}
int GetMonthDays(Date date) {
  bool Is31Days = ((date.month * 3) % 23) % 2;
  if (date.month == 2) return 28 + IsLeapYear(date.year);
  else return 30 + Is31Days;
}
int GetMonthWeeks(Date date, int week) {
  date.day = 1;
  int CurWeek = GetWeek(date);
  int diff = CurWeek >= week ? 7 + week - CurWeek : CurWeek - week;
  return (GetMonthDays(date) - diff) / 7 + 1;
}
int GetMonthWeek(Date date, int week, int index) {
  date.day = 1;
  int CurWeek = GetWeek(date);
  int first = (CurWeek >= week ? 7 + week - CurWeek : CurWeek - week) + 1;
  first += index * 7;
  return (first > GetMonthDays(date)) ? 0 : first;
}
/*
* @return true ,if A is earlier.
*/
bool Judge(Date A, Date B) {
  if (A.year < B.year) return true;
  else if (A.year > B.year) return false;
  else if (A.month < B.month) return true;
  else if (A.month > B.month) return false;
  else if (A.day <= B.day) return true;
  else return false;
}
/* Rata Die day one is 0001-01-01
* ������������������
*/
int ACDays(Date date) {
  if (date.month <= 2) {
    date.month += 12;
    date.year--;
  }
  int d = date.day;
  int y = date.year;
  int m = date.month;
  return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d - 306;
}
int DaysBetween(Date A, Date B) {
  return ACDays(B) - ACDays(A);
}

Date AddDays(Date date, int day) {
  Date res = date;
  if (day > 0) {
    res.day += day;
    while (res.day > GetMonthDays(res)) {
      res.day -= GetMonthDays(res);
      res.month++;
      if (res.month == 13) {
        res.month = 1;
        res.year++;
      }
    }
  } else {
    res.day += day;
    while (res.day <= 0) {
      res.month--;
      if (res.month == 0) {
        res.month = 12;
        res.year--;
      }
      res.day += GetMonthDays(res);
    }
  }
  return res;
}
String PrintDate(Date date) {
  String str = "";
  str += date.year;
  str += '-';
  str += date.month;
  str += '-';
  str += date.day;
  return str;
}
bool Check(Date date) {
  return (date.month >= 1 && date.month <= 12) && (date.day >= 1 && date.day <= GetMonthDays(date));
}
Date Trans(String str) {
  int y = str.substring(0, 4).toInt();
  int m = str.substring(5, 7).toInt();
  int d = str.substring(8).toInt();
  Date date = { y, m, d };
  return date;
}
Date GetLocalDate(DateTime dateTime) {
  Date date;
  date.year = dateTime.year();
  date.month = dateTime.month();
  date.day = dateTime.day();
  return date;
}
String GetLocalTime(DateTime dateTime){
  return (String)dateTime.hour()+':'+(String)dateTime.minute()+':'+(String)dateTime.second();
}