#pragma once
#include<Arduino.h>
struct Line{
  String content[2]; 
};
class Table{
  public:
  Line* tables;
  int num=0;
  void AddTable(String A,String B);
  int GetIndex(String value,int row);
  void DeleteTable(int line);
  void ChangeTable(String value,int row);
  static const int TableRows=2;
};