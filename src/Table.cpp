#include "Table.h"
void Table::AddTable(String A, String B) {
  num++;
  Line* t = new Line[num];
  for (int i = 0; i <= num - 2; i++) {
    t[i].content[0] = tables[i].content[0];
    t[i].content[1] = tables[i].content[1];
  };
  t[num-1].content[0]=A;
  t[num-1].content[1]=B;
  if(num!=1)delete[] tables;
  tables = t;
}
int Table::GetIndex(String value, int row) {
  for (int i = 0; i <= num - 1; i++) {
    if (value == tables[i].content[row]) return i;
  }
  return -1;
}
//line start at 1
void Table::DeleteTable(int line) {
  if (line < 0 || line > num) return;
  Line* t = new Line[num - 1];
  int index = 0;
  for (int i = 0; i <= line - 2; i++) {
    t[i].content[0] = tables[i].content[0];
    t[i].content[1] = tables[i].content[1];
  }
  for (int i = line; i <= num - 1; i++) {
    t[i-1].content[0] = tables[i].content[0];
    t[i-1].content[1] = tables[i].content[1];
  }
  delete[] tables;
  tables = t;
}
void Table::ChangeTable(String value, int row) {
  return;
}