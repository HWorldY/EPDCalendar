#include "WiFiController.h"
WiFiController::WiFiController(ESP8266WebServer* server) {
  this->server = server;
  IsInitAP = 0;
}
void WiFiController::InitAP(String ssid, String password) {
  this->APSSID = ssid;
  this->APPassword = password;
  this->IsInitAP = true;
}
void WiFiController::InitAP(const char* ssid, const char* password) {
  InitAP((String)ssid, (String)password);
}
String WiFiController::PrintSSID() {
  switch (WiFiMode) {
    case WIFI_AP:
      return WiFi.softAPSSID();
      break;
    case WIFI_STA:
      return WiFi.SSID();
    default:
      return (String) "";
  }
}
String WiFiController::PrintIP() {
  switch (WiFiMode) {
    case WIFI_AP:
      return WiFi.softAPIP().toString();
      break;
    case WIFI_STA:
      return WiFi.localIP().toString();
    default:
      return (String) "";
  }
}
void WiFiController::mode(WiFiMode_t mode) {
  if (this->WiFiMode == mode) return;
  server->close();
  if (mode == WIFI_OFF) {
    WiFi.mode(WIFI_OFF);
    SysState.SetState(ST_WIFI, 1);
  } else InitWiFi(mode);
}
void WiFiController::InitWiFi(WiFiMode_t mode,bool FirstTime) {
  if (mode == WIFI_AP_STA) return;
  int wait = 0;
  WiFi.mode(mode);
  switch (mode) {
    case WIFI_AP:
      WiFi.softAP(APSSID, APPassword);
      SysState.SetState(ST_WIFI, 0);
      break;
    case WIFI_STA:
      WiFi.begin(ssid, password);
      WiFi.hostname("EPDEditor");
      while (WiFi.status() != WL_CONNECTED) {
        if (wait < 15) wait++;
        else break;
        delay(500);
        Serial.print(".");
      }
      SysState.SetState(ST_WIFI, wait < 15 ? 0 : 1);
      Serial.println(WiFi.localIP().toString());
      break;
    default:
      return;
      break;
  }
  if(!FirstTime)server->begin();
  this->WiFiMode = mode;
}