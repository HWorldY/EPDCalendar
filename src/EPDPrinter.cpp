#include "EPDPrinter.h"
EPDPrinter::EPDPrinter(uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color)
  : Printer(Width, Height, Rotate, Color) {}
void EPDPrinter::EPDInit() {
  if (IsInit) return;
  Serial.println("-------------------------Start initializing e-paper");
  delay(200);
#ifdef COLOR_3
  DEV_Module_Init();
  EPD_4IN2_Init();
#else
  DEV_Module_Init();
  EPD_4IN2_Init_Fast();
#endif
  //EPD_4IN2_Clear();
  DEV_Delay_ms(500);
  Serial.println("--------------------Succeed To Initialize e-paper!");
  delay(200);
  IsInit = true;
  SysState.SetState(ST_EPD, 0);
}
void EPDPrinter::Refresh() {
#ifdef COLOR_3
  EPD_4IN2_SendImage();
  EPD_4IN2_TurnOnDisplay();
#else
  EPD_4IN2_Display(paint.Image);
  DEV_Delay_ms(2000);
#endif
}
void EPDPrinter::EPDSleep() {
  if (!IsInit) return;
#ifdef COLOR_3
  EPD_4IN2_Clear();
  EPD_4IN2_Sleep();
#else
  EPD_4IN2_Clear();
  EPD_4IN2_Sleep();
#endif
  Serial.println("E-paper sleep.");
  IsInit = false;
  delay(2000);
}
void EPDPrinter::PrintDate(int x, int y, Date date, CFONT* cf, sFONT* sf) {
  const char* s = ::PrintDate(date).c_str();
  DisplayString(CT_ENG, x, y, 0, 0, s, s, cf, sf, BLACK, WHITE);
}
void EPDPrinter::PrintSche(CodeKind ck, int x1, int y1, int x2, int y2, int gap, CFONT* cf, sFONT* sf, Schedule sche, Date date) {
  Table tb;
  int index = 0;
  for (int m = 0; m <= sche.rulenum - 1; m++) {
    if (sche.IsInRule(sche.rules[m], date)) {
      index++;
      tb.AddTable((String)index + '.', (String)sche.rules[m].str.c_str());
    }
  };
  DrawTable(ck, x1, y1, x2, y2, 48, &tb, nullptr, cf, sf, BLACK, WHITE);
  return;
}
bool EPDPrinter::IsIdle() {
  return digitalRead(EPD_BUSY_PIN);
}
#ifdef COLOR_3
void EPDPrinter::EPD_4IN2_SendImage() {
  uint16_t Width, Height;
  Width = (EPD_4IN2_WIDTH % 8 == 0) ? (EPD_4IN2_WIDTH / 8) : (EPD_4IN2_WIDTH / 8 + 1);
  Height = EPD_4IN2_HEIGHT;
  EPD_4IN2_SendCommand(0x10);
  for (uint16_t j = 0; j < Height; j++) {
    for (uint16_t i = 0; i < Width; i++) {
      if (!IsInRect(i * 8, j)) EPD_4IN2_SendData(paint.Image[i + j * Width]);
      else EPD_4IN2_SendData((unsigned char)0xFF);
    }
  };
  delay(100);
  EPD_4IN2_SendCommand(0x13);
  for (uint16_t j = 0; j < Height; j++) {
    for (uint16_t i = 0; i < Width; i++) {
      if (IsInRect(i * 8, j)) EPD_4IN2_SendData(paint.Image[i + j * Width]);
      else EPD_4IN2_SendData((unsigned char)0xFF);
    }
  };
}
void EPDPrinter::AddRect(int x1, int y1, int x2, int y2) {
  if (x2 <= 0) x2 += this->paint.Width;
  if (y2 <= 0) y2 += this->paint.Height;
  int num = ReNum;
  if (num >= 10) return;
  RedRe[num].x1 = x1;
  RedRe[num].y1 = y1;
  RedRe[num].x2 = x2;
  RedRe[num].y2 = y2;
  RedRe[num] = Convert(RedRe[num]);
  ReNum++;
}
bool EPDPrinter::IsInRect(uint16_t x, uint16_t y) {
  for (int i = 0; i <= ReNum - 1; i++) {
    if (IsPtInRect(RedRe[i], x, y)) return true;
  }
  return false;
}
#endif
bool IsPtInRect(RECT r, uint16_t x, uint16_t y) {
  return (r.x1 <= x && x <= r.x2 && r.y1 <= y && y <= r.y2) ? 1 : 0;
}