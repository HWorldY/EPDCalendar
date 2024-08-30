#pragma once
#include "ArduinoApp.h"
class SettingsDisplay : public ArduinoApp {
public:
  SettingsDisplay(RECT screen, short ID, ArduinoApp* parent = nullptr);
public:
  AVALUE Callback(short msg, AVALUE param1 = 0, AVALUE param2 = 0);
  AVALUE Run(short act = ACT_MAIN, AVALUE param1 = 0, AVALUE param2 = 0);
  AVALUE OnPaint();
  AVALUE OnKey();
  AVALUE OnChangeWiFiMode(AVALUE param1=0);
public:
  AnalogKey* AKey;
  ESP8266WebServer* server;
  WiFiController* WiFiCtr;
  EPDPrinter* print;
  RTC_DS1307* RTC;
  CFONT* sd;
};
