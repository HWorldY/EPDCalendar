#include "CalendarDisplay.h"
CalendarDisplay::CalendarDisplay(RECT screen, short ID, ArduinoApp* parent)
  : ArduinoApp(screen, ID, parent) {
}
ArduinoApp* CalendarDisplay::FindChild(short ID) {
  switch (ID) {
    case ID_CALENDAR:
      return this;
      break;
    default:
      return GetParent()->FindChild(ID);
  }
}
AVALUE CalendarDisplay::Callback(short msg, AVALUE param1, AVALUE param2) {
  switch (msg) {
    case MSG_INIT:
      if (IsInit == false) OnInit();
      else Serial.println("CalendarDisplay has been initialized!");
      break;
    case MSG_PAINT:
      OnPaint();
      break;
    default:
      break;
  }
  return 0;
}
AVALUE CalendarDisplay::Run(short act, AVALUE param1, AVALUE param2) {
  ContinueLoop = 1;
  this->print->Clear(WHITE);
  this->Callback(MSG_PAINT);
  KEYINFO kinfo;
  kinfo.id = KEY_NONE;
  while (kinfo.id == KEY_NONE && ContinueLoop) {
    if(this->print->IsIdle())kinfo = AKey->GetKey();
    //server->handleClient();
    delay(10);
  }
  switch (kinfo.id) {
    case KEY_1:
      ContinueLoop = 0;
      this->GetParent()->Callback(MSG_SWITCHPAGE, PAGE_SCHE);
      break;
    case KEY_2:
      ContinueLoop = 0;
      this->GetParent()->Callback(MSG_SWITCHPAGE, PAGE_SETT);
      break;
    default:
      break;
  }
  return 0;
}
AVALUE CalendarDisplay::OnInit() {
  Callback(MSG_PAINTDATE);
  return 0;
}
AVALUE CalendarDisplay::OnPaint() {
  String num;
  if (CurrentDate->day <= 9) num += ' ';
  num += CurrentDate->day;
  CFONTINFO cfinfo = this->sd->Change(96, "UNICODEH96.DZK");
  this->print->DisplayString(CT_COM, (this->screen.x1 + this->screen.x2) / 2 - 48, (this->screen.y1 + this->screen.y2) / 2 - 48, 0, 0,
                             num.c_str(), num.c_str(), sd, &Font20, BLACK, WHITE, nullptr, nullptr, 0, 0, sd->FontSize / 2);
  this->sd->Change(cfinfo.FontSize, cfinfo.path.c_str());
  String ch = (String) "待办：" + schedule->GetNum(*CurrentDate);
  String en = (String) "TODO:" + schedule->GetNum(*CurrentDate);
  this->print->DisplayString(CT_COM, 0, this->screen.y2 - 20, 0, 0, ch.c_str(), en.c_str(), sd, &Font20, BLACK, WHITE);
  this->print->Refresh();
  return 0;
}