#pragma once
#include <Arduino.h>
enum KeyID { KEY_NONE = 0,
             KEY_1,
             KEY_2,
             KEY_3,
             KEY_4
};
enum KeyMode { MODE_SHORT,
               MODE_LONG };
#define KeyNum 4
struct KEYINFO {
  KeyMode mode;
  short id;
};
class AnalogKey {
public:
  AnalogKey(int key);
  KEYINFO GetKey();
  static bool OutOfRange(int value);
  int key;
  constexpr static int Range[KeyNum + 2] = { 0, 500, 570, 740, 850, 1024 };
private:
  const int LongPressTime = 500;
};