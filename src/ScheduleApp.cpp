#include "ScheduleApp.h"
ScheduleApp::ScheduleApp(RECT screen, short ID, ArduinoApp* parent)
  : ArduinoApp(screen, ID, parent), ScheDis(screen, ID_SCHEDULE, this), SettDis(screen, ID_SETTINGS, this), CaleDis(screen, ID_CALENDAR, this) {
  this->CurrentPage = PAGE_SCHE;
}
ArduinoApp* ScheduleApp::FindChild(short ID) {
  switch (ID) {
    case ID_MAINAPP:
      return this;
      break;
    case ID_SCHEDULE:
      return &ScheDis;
      break;
    case ID_SETTINGS:
      return &SettDis;
      break;
    case ID_CALENDAR:
      return &CaleDis;
      break;
    default:
      return nullptr;
      break;
  }
  return nullptr;
}
AVALUE ScheduleApp::Callback(short msg, AVALUE param1, AVALUE param2) {
  switch (msg) {
    case MSG_INIT:
      if (IsInit == false) OnInit();
      else Serial.println("ScheduleApp has been initialized!");
      break;
    case MSG_REFRESHSCHE:
      OnRefreshSche();
      break;
    case MSG_PAINTDATE:
      OnPaintDate();
      break;
    case MSG_SLEEP:
      OnSleep();
      Callback(MSG_QUIT);
      break;
    case MSG_PAINT:
      OnPaint();
      break;
    case MSG_SENDDATE:
      this->CurrentDate = *(Date*)param1;
      break;
    case MSG_SWITCHPAGE:
      this->CurrentPage = param1;
      break;
    case MSG_QUIT:
      this->ID = 0;
      this->ScheDis.ContinueLoop=0;
      this->SettDis.ContinueLoop=0;
      this->CaleDis.ContinueLoop=0;
      break;
    default:
      break;
  }
  return 0;
}
AVALUE ScheduleApp::Run(short act, AVALUE param1, AVALUE param2) {
  switch (act) {
    case ACT_INIT:
      Callback(MSG_INIT);
      break;
    case ACT_MAIN:
      ContinueLoop = 1;
      #ifdef COLOR_3
      print->AddRect(0, 0, 299, 20);
      #endif
      this->print->Clear(WHITE);
      int test = 0;
      while (ContinueLoop) {
        switch (CurrentPage) {
          case PAGE_SCHE:
            ScheDis.Run();
            break;
          case PAGE_SETT:
            SettDis.Run();
            break;
          case PAGE_CALE:
            CaleDis.Run();
            break;
          default:
            break;
        }
      }
      return 0;
  }
  return 0;
}
AVALUE ScheduleApp::OnInit() {
  this->sd = new CFONT(18, "UNICODEH18.DZK");
  sd->Begin();
  ScheDis.sd = sd;
  SettDis.sd = sd;
  CaleDis.sd = sd;

  this->AKey = new AnalogKey(17);
  ScheDis.AKey = AKey;
  SettDis.AKey = AKey;
  CaleDis.AKey = AKey;

  schedule.Update();
  ScheDis.schedule = &schedule;
  CaleDis.schedule = &schedule;

  this->CurrentDate.year = 2024;
  this->CurrentDate.month = 8;
  this->CurrentDate.day = 1;
  OnGetDate();

  this->RTC=new RTC_DS1307;
  Wire.begin();  // 开启总线，这个用于I2C的使用
  RTC->begin();   // 初始化时钟
  SettDis.RTC = RTC;
  ScheDis.RTC = RTC;
  CaleDis.RTC = RTC;
  if(this->CurrentDate.day==0)CurrentDate=GetLocalDate(this->RTC->now());
  Serial.println(PrintDate(GetLocalDate(this->RTC->now())));
  //DateTime dt(2024,8,17,17,38,0);
  //RTC->adjust(dt);
  
  ScheDis.CurrentDate = &CurrentDate;
  CaleDis.CurrentDate = &CurrentDate;

  this->print = new EPDPrinter(EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 270, WHITE);
  this->print->EPDInit();
  ScheDis.print = print;
  SettDis.print = print;
  CaleDis.print = print;

  SettDis.server = server;
  ScheDis.server = server;

  SettDis.WiFiCtr = WiFiCtr;
  ScheDis.WiFiCtr = WiFiCtr;
  CaleDis.WiFiCtr = WiFiCtr;


  this->IsInit = true;

  return 0;
}
AVALUE ScheduleApp::OnPaint() {
  switch (CurrentPage) {
    case PAGE_SCHE:
      this->ScheDis.Run();
    default:
      break;
  }
  return 0;
}
AVALUE ScheduleApp::OnRefreshSche() {
  this->ScheDis.Run();
  return 0;
}
AVALUE ScheduleApp::OnPaintDate() {
  this->print->PrintDate(0, 0, CurrentDate, sd, &Font20);
  this->print->Refresh();
  return 0;
}
AVALUE ScheduleApp::OnSleep() {
  print->EPDSleep();
  this->ContinueLoop = false;
  SysState.SetState(ST_EPD, 1);
  return 0;
}
AVALUE ScheduleApp::OnGetDate() {
  String t = GetDate();
  int length = t.length();
  int num = 0;
  String time;
  for (int index = 0; index <= length - 1; index++) {
    if (t[index] == '\"') num++;
    if (num == 7) {
      time = t.substring(index + 1, index + 15);
      break;
    }
  }
  Serial.println(time.c_str());
  Date* date = new Date;
  date->year = time.substring(0, 4).toInt();
  date->month = time.substring(4, 6).toInt();
  date->day = time.substring(6, 8).toInt();
  Serial.println(PrintDate(*date));
  this->CurrentDate = *date;
  return 0;
  
}
/*int lines = 65;
  int bytesperline = 30;
  int byte = 0;
  for (int line = 0; line <= lines - 1; line++) {
    for (int pos = 0; pos <= bytesperline - 1; pos++) {
      for (int i = 7; i >= 0; i--) {
        Serial.print(((print->paint.Image[byte] >> i) & 1) ? "  " : "##");
      }
      byte++;
    }
    Serial.print('\n');
  }*/