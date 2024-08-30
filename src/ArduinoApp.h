#pragma once
#include"AnalogKey.h"
#include "EPDPrinter.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include"WiFiController.h"
#include"State.h"
#include <Wire.h>  //IIC库
#include <RTClib.h>
typedef long long int AVALUE;
enum MESSAGE {
  MSG_KEY,
  MSG_INIT,
  MSG_CREATE,
  MSG_PAINT,
  MSG_QUIT,
  MSG_PAINTDATE,
  MSG_PAINTSCHE,
  MSG_SLEEP,
  MSG_REFRESHSCHE,
  MSG_SENDDATE,
  MSG_SWITCHPAGE,
  MSG_CHANGEWIFIMODE
};
enum CHILDID {
  ID_MAINAPP,
  ID_SCHEDULE,
  ID_SETTINGS,
  ID_CALENDAR
};

enum ACTION {
  ACT_INIT,
  ACT_MAIN,
  ACT_SCHE,
  ACT_CAL,
  ACT_SET,
};
enum PAGE_FOR_SCHEDULEAPP {
  PAGE_SCHE,
  PAGE_SETT,
  PAGE_CALE
};
class ArduinoApp {
public:
  bool IsInit = false;
  bool ContinueLoop = 1;
  EPDPrinter* print;
  RECT screen;
  short ID;
  ArduinoApp* GetParent();
  virtual ArduinoApp* FindChild(short ID);
public:
  ArduinoApp(RECT screen, short ID, ArduinoApp* parent = nullptr);
  virtual AVALUE Run(short act = ACT_MAIN, AVALUE param1 = 0, AVALUE param2 = 0);
  virtual AVALUE Callback(short msg, AVALUE param1 = 0, AVALUE param2 = 0);
private:
  ArduinoApp* parent = nullptr;
};