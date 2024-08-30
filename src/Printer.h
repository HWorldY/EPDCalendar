#pragma once
#include "EPD_4in2bc.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "State.h"
#include "Table.h"
struct RECT {
  uint16_t x1;
  uint16_t y1;
  uint16_t x2;
  uint16_t y2;
};
struct POINT {
  uint16_t x;
  uint16_t y;
};
class Printer {
public:
  PAINT paint;
  Printer();
  Printer(uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color);
  virtual POINT Convert(uint16_t x, uint16_t y);
  virtual RECT Convert(RECT r);
  virtual void NewImage(uint8_t* image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color);
  virtual void SelectImage(uint8_t* image);
  virtual void SetRotate(uint16_t Rotate);
  virtual void SetMirroring(uint8_t mirror);
  virtual void SetPixel(uint16_t XPoint, uint16_t YPoint, uint16_t Color);
  virtual void SetScale(uint8_t scale);
  virtual void Clear(uint16_t Color);
  virtual void ClearWindows(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t Color);
  //Drawing
  virtual void DrawPoint(uint16_t XPoint, uint16_t YPoint, uint16_t Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
  virtual void DrawLine(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style);
  virtual void DrawRectangle(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);
  virtual void DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);
  //Display string
  virtual uint32_t GetOffset(CodeKind ck, void* ch, void* fo);
  virtual void DrawBuffer(uint16_t XPoint, uint16_t YPoint, const unsigned char* ptr, uint16_t Width, uint16_t Height, uint16_t Color_Foreground, uint16_t Color_Background);
  virtual void DrawBufferSD(uint16_t XPoint, uint16_t YPoint, unsigned long long int pos, File file, uint16_t Width, uint16_t Height, uint16_t Color_Foreground, uint16_t Color_Background);
  virtual void DrawChar(uint16_t XStart, uint16_t YStart, const char Acsii_Char, sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background);
  virtual void DrawString_UTF(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, unsigned char* utf8,
                              CFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background, int* XNew = nullptr, int* YNew = nullptr, bool NextPos = 0, bool NextLine = 0, uint16_t XLimit = 0);
  virtual void DrawString_EN(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, const char* pString,
                             sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background, int* XNew = nullptr, int* YNew = nullptr, bool NextPos = 0, bool NextLine = 0);
  virtual void DrawString_CN(uint16_t XStart, uint16_t YStart, const char* pString, cFONT* font, uint16_t Color_Foreground, uint16_t Color_Background);
  virtual void DisplayString(CodeKind ck, uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, const void* utf, const void* eng,
                             CFONT* cf, sFONT* sf, uint16_t Color_Foreground, uint16_t Color_Background, int* XNew = nullptr, int* YNew = nullptr, bool NextPos = 0, bool NextLine = 0, uint16_t XLimit = 0);
  virtual void DrawNum(uint16_t XPoint, uint16_t YPoint, int32_t Nummber, sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background);
  //pic
  virtual void DrawBitMap(const unsigned char* image_buffer);
  virtual void DrawImage(const unsigned char* image_buffer, uint16_t XStart, uint16_t YStart, uint16_t W_Image, uint16_t H_Image);
  //tab
  virtual void DrawTable_EN(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t MidLine, Table* eng,
                            sFONT* sf, uint16_t Color_Foreground, uint16_t Color_Background);
  virtual void DrawTable_UTF(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t MidLine, Table* utf,
                             CFONT* cf, uint16_t Color_Foreground, uint16_t Color_Background);
  virtual void DrawTable(CodeKind ck, uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t MidLine, Table* utf, Table* eng,
                         CFONT* cf, sFONT* sf, uint16_t Color_Foreground, uint16_t Color_Background);
};