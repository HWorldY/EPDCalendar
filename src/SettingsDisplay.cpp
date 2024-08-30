#include "SettingsDisplay.h"
SettingsDisplay::SettingsDisplay(RECT screen, short ID, ArduinoApp* parent)
  : ArduinoApp(screen, ID, parent) {
}
AVALUE SettingsDisplay::Callback(short msg, AVALUE param1, AVALUE param2) {
  switch (msg) {
    case MSG_PAINT:
      this->print->Clear(WHITE);
      OnPaint();
      break;
    case MSG_KEY:
      return OnKey();
      break;
    case MSG_CHANGEWIFIMODE:
      OnChangeWiFiMode(param1);
      break;
    default:
      break;
  }
  return 0;
}
String TransWiFiMode(WiFiMode_t wfm, bool utf) {
  switch (wfm) {
    case WIFI_AP:
      return utf ? "AP模式" : "AP Mode";
      break;
    case WIFI_STA:
      return utf ? "STA模式" : "STA Mode";
      break;
    case WIFI_AP_STA:
      return utf ? "AP+STA模式" : "AP+STA Mode";
      break;
    default:
      return utf ? "无" : "None";
  }
}
AVALUE SettingsDisplay::OnPaint() {
  this->print->DisplayString(CT_COM, 0, 0, 0, 0, "设置", "Settings", this->sd, &Font24, BLACK, WHITE);
  this->print->DrawLine(0, 20, this->screen.x2 - 1, 20, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Table ctb;
  Table etb;
  etb.AddTable((String) "WiFi", (String)(SysState.GetState(ST_WIFI) ? "Off" : "On"));
  etb.AddTable((String) "WIFI-Mode", TransWiFiMode(WiFi.getMode(), 0));
  etb.AddTable((String) "WiFi-Name", WiFiCtr->PrintSSID());
  etb.AddTable((String) "IP address", WiFiCtr->PrintIP());
  etb.AddTable((String) "SD", (String)(SysState.GetState(ST_SD) ? "Failed" : "Success"));
  etb.AddTable((String) "Font", (String)sd->FontFile.name());

  ctb.AddTable((String) "WiFi", (String)(SysState.GetState(ST_WIFI) ? "关闭" : "打开"));
  ctb.AddTable((String) "WIFI模式", TransWiFiMode(WiFi.getMode(), 1));
  ctb.AddTable((String) "WiFi名称", WiFiCtr->PrintSSID());
  ctb.AddTable((String) "IP地址", WiFiCtr->PrintIP());
  ctb.AddTable((String) "SD", (String)(SysState.GetState(ST_SD) ? "失败" : "成功"));
  ctb.AddTable((String) "字体", (String)sd->FontFile.name());
  this->print->DrawTable(CT_COM, 0, 25, 0, 0, 120, &ctb, &etb, sd, &Font20, BLACK, WHITE);
  this->print->Refresh();
  return 0;
}
AVALUE SettingsDisplay::Run(short act, AVALUE param1, AVALUE param2) {
  ContinueLoop = 1;
  this->print->Clear(WHITE);
  this->Callback(MSG_PAINT);
  KEYINFO kinfo;
  kinfo.id = KEY_NONE;
  while (ContinueLoop) {
    if(this->print->IsIdle())ContinueLoop = Callback(MSG_KEY);
    server->handleClient();
    delay(10);
  }
  return 0;
}
AVALUE SettingsDisplay::OnKey() {
  KEYINFO kinfo = AKey->GetKey();
  switch (kinfo.id) {
    case KEY_1:
      this->GetParent()->Callback(MSG_SWITCHPAGE, PAGE_CALE);
      return 0;
      break;
    case KEY_2:
      this->GetParent()->Callback(MSG_SWITCHPAGE, PAGE_SCHE);
      return 0;
      break;
    case KEY_3:
      Callback(MSG_CHANGEWIFIMODE, kinfo.mode);
      Callback(MSG_PAINT);
      return 1;
      break;
    default:
      break;
  }
  return 1;
}
AVALUE SettingsDisplay::OnChangeWiFiMode(AVALUE param1) {
  if (param1 == MODE_SHORT) {
    int wmode = WiFi.getMode();
    if (wmode == WIFI_STA) {
      WiFiCtr->mode(WIFI_AP);
    }
    if (wmode == WIFI_AP) {
      WiFiCtr->mode(WIFI_STA);
    }
  } else if (param1 == MODE_LONG) {
    if (!SysState.GetState(ST_WIFI)) WiFiCtr->mode(WIFI_OFF);
    else WiFiCtr->mode(WiFiCtr->GetMode());
  }
  return 0;
}