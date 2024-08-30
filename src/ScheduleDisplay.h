#pragma once
#include "ArduinoApp.h"
class ScheduleDisplay : public ArduinoApp {
public:
public:
  ScheduleDisplay(RECT screen, short ID, ArduinoApp* parent = nullptr);
  ArduinoApp* FindChild(short ID);
public:
  AVALUE Callback(short msg, AVALUE param1 = 0, AVALUE param2 = 0);
  AVALUE Run(short act = ACT_MAIN, AVALUE param1 = 0, AVALUE param2 = 0);
private:
  AVALUE OnInit();
  AVALUE OnPaint();
  AVALUE OnKey();
public:
  AnalogKey* AKey;
  ESP8266WebServer* server;
  WiFiController* WiFiCtr;
  EPDPrinter* print;
  RTC_DS1307* RTC;
  CFONT* sd;
public:
  Schedule* schedule;
  Date* CurrentDate;
private:
  short CurrentAction;
};