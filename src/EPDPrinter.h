#pragma once
#include <Arduino.h>
#include "Printer.h"
#include "Schedule.h"
class EPDPrinter : public Printer {
public:
  EPDPrinter(uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color);
  void EPDInit();
  void Refresh();
  void EPDSleep();
  bool IsIdle();  //ReadBusy

  void PrintDate(int x, int y, Date date, CFONT* cf, sFONT* sf);
  void PrintSche(CodeKind ck, int x1, int y1, int x2, int y2, int gap, CFONT* cf, sFONT* sf, Schedule sche, Date date);

#ifdef COLOR_3
  bool IsInRect(uint16_t x, uint16_t y);
  void AddRect(int x1, int y1, int x2, int y2);
private:
  void EPD_4IN2_SendImage();
  RECT RedRe[10] = {};
  int ReNum = 0;
#endif
private:
  bool IsInit = 0;
};
bool IsPtInRect(RECT r, uint16_t x, uint16_t y);