#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include "State.h"
class WiFiController {
public:
  WiFiController(ESP8266WebServer* server);
  void InitAP(String ssid, String password);
  void InitAP(const char* ssid, const char* password);
  String PrintSSID();
  String PrintIP();
  WiFiMode_t GetMode() {
    return WiFiMode;
  }
  void mode(WiFiMode_t mode);
  void InitWiFi(WiFiMode_t mode,bool FirstTime=0);
private:
  bool IsInitAP;
  String APSSID;
  String APPassword;
  String ssid = "HiWiFi_he";
  String password = "Hejun12345@";
  ESP8266WebServer* server;
  WiFiMode_t WiFiMode = WIFI_OFF;
};