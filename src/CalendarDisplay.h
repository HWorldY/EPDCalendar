#pragma once
#include "ArduinoApp.h"
class CalendarDisplay : public ArduinoApp {
public:
public:
  CalendarDisplay(RECT screen, short ID, ArduinoApp* parent = nullptr);
  ArduinoApp* FindChild(short ID);
public:
  AVALUE Callback(short msg, AVALUE param1 = 0, AVALUE param2 = 0);
  AVALUE Run(short act = ACT_MAIN, AVALUE param1 = 0, AVALUE param2 = 0);
private:
  AVALUE OnInit();
  AVALUE OnCreate();
  AVALUE OnPaint();
  AVALUE OnPaintSche();
  AVALUE OnPaintDate();
public:
  AnalogKey* AKey;
  WiFiController* WiFiCtr;
  RTC_DS1307* RTC;
  EPDPrinter* print;
  CFONT* sd;
public:
  Schedule* schedule;
  Date* CurrentDate;
private:
  short CurrentAction;
};