#include "Schedule.h"
String PrintRule(Rule rule) {
  String str;
  str += PrintDate(rule.start);
  str += "~";
  str += PrintDate(rule.end);
  str += "~";
  str += rule.mode;
  str += "-";
  str += rule.value;
  str += "~";
  str += rule.str.c_str();
  return str;
}
Schedule::Schedule() {
}
int Schedule::GetNum(Date date) {
  int n = 0;
  for (int m = 0; m <= rulenum - 1; m++) {
    if (IsInRule(rules[m], date)) {
      n++;
    }
  }
  return n;
}
bool Schedule::IsInRule(Rule r, Date d) {
  if (Judge(r.start, d) && Judge(d, r.end)) {
    if (r.mode == WEEK) {
      if (r.value == GetWeek(d)) return true;
    } else {
      if (r.value == 1) return true;
      if (DaysBetween(r.start, d) % r.value == 0) return true;
    }
  }
  return false;
}
String Schedule::Print(Date date) {
  String str;
  for (int m = 0; m <= rulenum - 1; m++) {
    if (IsInRule(rules[m], date)) {
      str += rules[m].str.c_str();
      str += ' ';
    }
  }
  return str;
}
String Schedule::PrintAll() {
  String str;
  for (int m = 0; m <= rulenum - 1; m++) {
    str += PrintRule(this->rules[m]);
    str += ' ';
  }
  return str;
}
bool Schedule::RemoveRule(String str) {
  int i = 0;
  for (; i <= this->rulenum - 1; i++) {
    if (str == PrintRule(this->rules[i])) break;
  }
  if (i == rulenum) return false;
  else {
    for (int m = i; m <= this->rulenum - 1; m++) {
      if (m == rulenum - 1) {
        this->rulenum--;
        break;
      }
      this->rules[i] = this->rules[i + 1];
    }
  }
  return true;
}
bool Schedule::RemoveRule(int index) {
  return true;
}
bool Schedule::CheckRule(Rule r) {
  if (Judge(r.start, r.end)) {
    if (r.mode == WEEK) {
      if (1 <= r.value && r.value <= 7) return true;
    } else if (r.value >= 1) return true;
  }
  return false;
}
void Schedule::Add(Date start, Date end, int mode, int value, string str) {
  Rule rule;
  rule.start = start;
  rule.end = end;
  rule.value = value;
  rule.mode = mode;
  if (CheckRule(rule) && rulenum <= MAX_RULE - 1) {
    rule.str = str;
    rules[rulenum] = rule;
    rulenum++;
  }
}
void Schedule::Read(String res) {
  //Clear();
  int syear;
  int smonth;
  int sday;
  int eyear;
  int emonth;
  int eday;
  int mode;
  int value;
  String str;
  int length = res.length();
  int Last = 0;
  int process = 0;
  for (int i = 0; i <= length - 1; i++) {
    if (res[i] == '-') {
      switch (process) {
        case 0:
          syear = res.substring(Last, i).toInt();
          break;
        case 1:
          smonth = res.substring(Last, i).toInt();
          break;
        case 2:
          sday = res.substring(Last, i).toInt();
          break;
        case 3:
          eyear = res.substring(Last, i).toInt();
          break;
        case 4:
          emonth = res.substring(Last, i).toInt();
          break;
        case 5:
          eday = res.substring(Last, i).toInt();
          break;
        case 6:
          mode = res.substring(Last, i).toInt();
          break;
        case 7:
          value = res.substring(Last, i).toInt();
          break;
        default:
          break;
      }
      process++;
      Last = i + 1;
    } else if (res[i] == ' ' || i == length - 1) {
      process = 0;
      str = res.substring(Last, i);
      Add(NewDate(syear, smonth, sday), NewDate(eyear, emonth, eday), mode, value, str.c_str());
        Last = i + 1;
    }
  }
}
String Schedule::Write() {
  String wstr;
  for (int m = 0; m <= rulenum - 1; m++) {
    Rule rule = this->rules[m];
    String str;
    str += PrintDate(rule.start);
    str += "-";
    str += PrintDate(rule.end);
    str += "-";
    str += rule.mode;
    str += "-";
    str += rule.value;
    str += "-";
    str += rule.str.c_str();
    wstr += str;
    wstr += ' ';
  }
  return wstr;
}
bool Schedule::Save() {
  File file = LittleFS.open("/Schedule.txt", "w");
  if (!file) {
    Serial.println((String) "Failed to open Schedule.txt");
    return 0;
  }
  file.print(Write());
  file.close();
  return 1;
}
bool Schedule::Update() {
  rulenum = 0;
  String res;
  File file = LittleFS.open("/Schedule.txt", "r");
  if (!file) {
    Serial.println((String) "Failed to open Schedule.txt");
    return 0;
  }
  while (file.available()) {
    res += (char)file.read();
  }
  file.close();
  Read(res);
  return 1;
}