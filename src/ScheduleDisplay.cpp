#include "ScheduleDisplay.h"
ScheduleDisplay::ScheduleDisplay(RECT screen, short ID, ArduinoApp* parent)
  : ArduinoApp(screen, ID, parent) {
}
ArduinoApp* ScheduleDisplay::FindChild(short ID) {
  switch (ID) {
    case ID_SCHEDULE:
      return this;
      break;
    default:
      return GetParent()->FindChild(ID);
  }
}
AVALUE ScheduleDisplay::Callback(short msg, AVALUE param1, AVALUE param2) {
  switch (msg) {
    case MSG_INIT:
      if (IsInit == false) OnInit();
      else Serial.println("ScheduleDisplay has been initialized!");
      break;
    case MSG_KEY:
      return OnKey();
      break;
    case MSG_PAINT:
      OnPaint();
      break;
    default:
      break;
  }
  return 0;
}
AVALUE ScheduleDisplay::Run(short act, AVALUE param1, AVALUE param2) {
  ContinueLoop = 1;
  Callback(MSG_PAINT);
  while (ContinueLoop) {
    if(this->print->IsIdle())ContinueLoop = Callback(MSG_KEY);
    server->handleClient();
    delay(10);
  }
  return 0;
}
AVALUE ScheduleDisplay::OnInit() {
  Callback(MSG_PAINTDATE);
  return 0;
}
AVALUE ScheduleDisplay::OnPaint() {
  this->print->Clear(WHITE);
  this->print->DisplayString(CT_COM, 0, 0, 0, 0, "待办", "TODO", sd, &Font20, BLACK, WHITE);
  this->print->DrawLine(0, 20, this->screen.x2 - 1, 20, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  this->print->PrintSche(CT_UTF, 0, 25, this->screen.x2, this->screen.y2, 5, sd, &Font20, *schedule, *CurrentDate);
  this->print->Refresh();
  return 0;
}
AVALUE ScheduleDisplay::OnKey() {
  KEYINFO kinfo = AKey->GetKey();
  switch (kinfo.id) {
    case KEY_1:
      this->GetParent()->Callback(MSG_SWITCHPAGE, PAGE_SETT);
      return 0;
    case KEY_2:
      this->GetParent()->Callback(MSG_SWITCHPAGE, PAGE_CALE);
      return 0;
      break;
    default:
      break;
  }
  return 1;
}