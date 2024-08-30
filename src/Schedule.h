#pragma once
#include"Date.h"
#include"LittleFS.h"
#define MAX_RULE 15
enum RuleMode {WEEK,DAY};
struct Rule{
  short mode;
  int value;
  Date start;
  Date end;
  string str;
};
String PrintRule(Rule rule);
class Schedule {
public:
	Schedule();
  bool IsInRule(Rule r,Date d);
  bool CheckRule(Rule r);
  int GetNum(Date date);
  void Add(Date start,Date end,int mode,int value,string str);
  String Print(Date date); 
  String PrintAll();
  bool RemoveRule(String str);
  bool RemoveRule(int index);

  void Read(String res);
  String Write();

  bool Save();
  bool Update();

  Rule rules[MAX_RULE];
  int rulenum=0;
};