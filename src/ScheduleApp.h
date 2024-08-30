#pragma once
#include "CFONT.h"
#include "ArduinoApp.h"
#include "ScheduleDisplay.h"
#include "SettingsDisplay.h"
#include "CalendarDisplay.h"
#include "GetDate.h"

class ScheduleApp : public ArduinoApp {
public:
  ScheduleApp(RECT screen, short ID, ArduinoApp* parent = nullptr);
  ArduinoApp* FindChild(short ID);
public:
  AVALUE Callback(short msg, AVALUE param1 = 0, AVALUE param2 = 0);
  AVALUE Run(short act = ACT_MAIN, AVALUE param1 = 0, AVALUE param2 = 0);
private:
  AVALUE OnInit();
  AVALUE OnPaint();
  AVALUE OnRefreshSche();
  AVALUE OnPaintDate();
  AVALUE OnSleep();
  AVALUE OnGetDate();
public:
  ScheduleDisplay ScheDis;
  SettingsDisplay SettDis;
  CalendarDisplay CaleDis;
public:
  AnalogKey* AKey;
  WiFiController* WiFiCtr;
  ESP8266WebServer* server;
  RTC_DS1307* RTC;
  EPDPrinter* print;
  CFONT* sd;
public:
  Schedule schedule;
  Date CurrentDate;
private:
  short CurrentAction;
  short CurrentPage;
};