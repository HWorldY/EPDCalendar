#include "Printer.h"
Printer::Printer() {
  return;
}
Printer::Printer(uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color) {
  uint8_t* BlackImage = nullptr;
  uint16_t Imagesize = ((Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1)) * Height;
  if ((BlackImage = (uint8_t*)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
  }
  this->NewImage(BlackImage, Width, Height, Rotate, Color);
}
POINT Printer::Convert(uint16_t XPoint, uint16_t YPoint) {
  if (XPoint > paint.Width || YPoint > paint.Height) {
    Serial.println("Exceeding display boundaries\r\n");
    return POINT();
  }
  uint16_t X, Y;
  switch (paint.Rotate) {
    case 0:
      X = XPoint;
      Y = YPoint;
      break;
    case 90:
      X = paint.WidthMemory - YPoint - 1;
      Y = XPoint;
      break;
    case 180:
      X = paint.WidthMemory - XPoint - 1;
      Y = paint.HeightMemory - YPoint - 1;
      break;
    case 270:
      X = YPoint;
      Y = paint.HeightMemory - XPoint - 1;
      break;
    default:
      return POINT();
  }

  switch (paint.Mirror) {
    case MIRROR_NONE:
      break;
    case MIRROR_HORIZONTAL:
      X = paint.WidthMemory - X - 1;
      break;
    case MIRROR_VERTICAL:
      Y = paint.HeightMemory - Y - 1;
      break;
    case MIRROR_ORIGIN:
      X = paint.WidthMemory - X - 1;
      Y = paint.HeightMemory - Y - 1;
      break;
    default:
      return POINT();
  }
  POINT pt = { X, Y };
  return pt;
}
RECT Printer::Convert(RECT r) {
  RECT re;
  POINT ps, pe;
  ps = Convert(r.x1, r.y1);
  pe = Convert(r.x2, r.y2);
  re.x1 = ps.x;
  re.y1 = ps.y;
  re.x2 = pe.x;
  re.y2 = pe.y;
  if (re.x1 > re.x2) {
    uint16_t x = re.x2;
    re.x2 = re.x1;
    re.x1 = x;
  }
  if (re.y1 > re.y2) {
    uint16_t y = re.y2;
    re.y2 = re.y1;
    re.y1 = y;
  }
  return re;
}
void Printer::NewImage(uint8_t* image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color) {
  paint.Image = NULL;
  paint.Image = image;

  paint.WidthMemory = Width;
  paint.HeightMemory = Height;
  paint.Color = Color;
  paint.Scale = 2;
  paint.WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
  paint.HeightByte = Height;
  paint.Rotate = Rotate;
  paint.Mirror = MIRROR_NONE;
  if (Rotate == ROTATE_0 || Rotate == ROTATE_180) {
    paint.Width = Width;
    paint.Height = Height;
  } else {
    paint.Width = Height;
    paint.Height = Width;
  }
}
/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Printer::SelectImage(uint8_t* image) {
  paint.Image = image;
}
/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/
void Printer::SetRotate(uint16_t Rotate) {
  if (Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
    // Serial.println("Set image Rotate %d\r\n", Rotate);
    paint.Rotate = Rotate;
  } else {
    Serial.println("rotate = 0, 90, 180, 270\r\n");
  }
}
/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Printer::SetMirroring(uint8_t mirror) {
  if (mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
    paint.Mirror = mirror;
  } else {
    Serial.println("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL,MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
  }
}
void Printer::SetScale(uint8_t scale) {
  if (scale == 2) {
    paint.Scale = scale;
    paint.WidthByte = (paint.WidthMemory % 8 == 0) ? (paint.WidthMemory / 8) : (paint.WidthMemory / 8 + 1);
  } else if (scale == 4) {
    paint.Scale = scale;
    paint.WidthByte = (paint.WidthMemory % 4 == 0) ? (paint.WidthMemory / 4) : (paint.WidthMemory / 4 + 1);
  } else if (scale == 7) {  //Only applicable with 5in65 e-Paper
    paint.Scale = 7;
    paint.WidthByte = (paint.WidthMemory % 2 == 0) ? (paint.WidthMemory / 2) : (paint.WidthMemory / 2 + 1);
  } else {
    Serial.println("Set Scale Input parameter error\r\n");
    Serial.println("Scale Only support: 2 4 7\r\n");
  }
}
/******************************************************************************
function: Draw Pixels
parameter:
    XPoint : At point X
    YPoint : At point Y
    Color  : Painted colors
******************************************************************************/
void Printer::SetPixel(uint16_t XPoint, uint16_t YPoint, uint16_t Color) {
  if (XPoint > paint.Width || YPoint > paint.Height) {
    Serial.println("Exceeding display boundaries\r\n");
    return;
  }
  uint16_t X, Y;
  switch (paint.Rotate) {
    case 0:
      X = XPoint;
      Y = YPoint;
      break;
    case 90:
      X = paint.WidthMemory - YPoint - 1;
      Y = XPoint;
      break;
    case 180:
      X = paint.WidthMemory - XPoint - 1;
      Y = paint.HeightMemory - YPoint - 1;
      break;
    case 270:
      X = YPoint;
      Y = paint.HeightMemory - XPoint - 1;
      break;
    default:
      return;
  }

  switch (paint.Mirror) {
    case MIRROR_NONE:
      break;
    case MIRROR_HORIZONTAL:
      X = paint.WidthMemory - X - 1;
      break;
    case MIRROR_VERTICAL:
      Y = paint.HeightMemory - Y - 1;
      break;
    case MIRROR_ORIGIN:
      X = paint.WidthMemory - X - 1;
      Y = paint.HeightMemory - Y - 1;
      break;
    default:
      return;
  }

  if (X > paint.WidthMemory || Y > paint.HeightMemory) {
    Serial.println("Exceeding display boundaries\r\n");
    return;
  }

  if (paint.Scale == 2) {
    uint32_t Addr = X / 8 + Y * paint.WidthByte;
    uint8_t Rdata = paint.Image[Addr];
    if (Color == BLACK)
      paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
    else
      paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
  } else if (paint.Scale == 4) {
    uint32_t Addr = X / 4 + Y * paint.WidthByte;
    Color = Color % 4;  //Guaranteed color scale is 4  --- 0~3
    uint8_t Rdata = paint.Image[Addr];

    Rdata = Rdata & (~(0xC0 >> ((X % 4) * 2)));
    paint.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4) * 2));
  } else if (paint.Scale == 7) {
    uint16_t Width = paint.WidthMemory * 3 % 8 == 0 ? paint.WidthMemory * 3 / 8 : paint.WidthMemory * 3 / 8 + 1;
    uint32_t Addr = (XPoint * 3) / 8 + YPoint * Width;
    uint8_t shift, Rdata, Rdata2;
    shift = (XPoint + YPoint * paint.HeightMemory) % 8;

    switch (shift) {
      case 0:
        Rdata = paint.Image[Addr] & 0x1f;
        Rdata = Rdata | ((Color << 5) & 0xe0);
        paint.Image[Addr] = Rdata;
        break;
      case 1:
        Rdata = paint.Image[Addr] & 0xe3;
        Rdata = Rdata | ((Color << 2) & 0x1c);
        paint.Image[Addr] = Rdata;
        break;
      case 2:
        Rdata = paint.Image[Addr] & 0xfc;
        Rdata2 = paint.Image[Addr + 1] & 0x7f;
        Rdata = Rdata | ((Color >> 1) & 0x03);
        Rdata2 = Rdata2 | ((Color << 7) & 0x80);
        paint.Image[Addr] = Rdata;
        paint.Image[Addr + 1] = Rdata2;
        break;
      case 3:
        Rdata = paint.Image[Addr] & 0x8f;
        Rdata = Rdata | ((Color << 4) & 0x70);
        paint.Image[Addr] = Rdata;
        break;
      case 4:
        Rdata = paint.Image[Addr] & 0xf1;
        Rdata = Rdata | ((Color << 1) & 0x0e);
        paint.Image[Addr] = Rdata;
        break;
      case 5:
        Rdata = paint.Image[Addr] & 0xfe;
        Rdata2 = paint.Image[Addr + 1] & 0x3f;
        Rdata = Rdata | ((Color >> 2) & 0x01);
        Rdata2 = Rdata2 | ((Color << 6) & 0xc0);
        paint.Image[Addr] = Rdata;
        paint.Image[Addr + 1] = Rdata2;
        break;
      case 6:
        Rdata = paint.Image[Addr] & 0xc7;
        Rdata = Rdata | ((Color << 3) & 0x38);
        paint.Image[Addr] = Rdata;
        break;
      case 7:
        Rdata = paint.Image[Addr] & 0xf8;
        Rdata = Rdata | (Color & 0x07);
        paint.Image[Addr] = Rdata;
        break;
    }
  }
}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Printer::Clear(uint16_t Color) {

  if (paint.Scale == 2 || paint.Scale == 4) {
    for (uint16_t Y = 0; Y < paint.HeightByte; Y++) {
      for (uint16_t X = 0; X < paint.WidthByte; X++) {  //8 pixel =  1 byte
        uint32_t Addr = X + Y * paint.WidthByte;
        paint.Image[Addr] = Color;
      }
    }
  } else if (paint.Scale == 7) {
    Color = (uint8_t)Color;
    uint16_t Width = (paint.WidthMemory * 3 % 8 == 0) ? (paint.WidthMemory * 3 / 8) : (paint.WidthMemory * 3 / 8 + 1);
    for (uint16_t Y = 0; Y < paint.HeightMemory; Y++) {
      for (uint16_t X = 0; X < Width; X++) {
        uint32_t Addr = X + Y * Width;
        if (Addr % 3 == 0)
          paint.Image[Addr] = ((Color << 5) | (Color << 2) | (Color >> 1));
        else if (Addr % 3 == 1)
          paint.Image[Addr] = ((Color << 7) | (Color << 4) | (Color << 1) | (Color >> 2));
        else if (Addr % 3 == 2)
          paint.Image[Addr] = ((Color << 6) | (Color << 3) | Color);
      }
    }
  }
}

/******************************************************************************
function: Clear the color of a window
parameter:
    XStart : x starting point
    YStart : Y starting point
    XEnd   : x end point
    YEnd   : y end point
    Color  : Painted colors
******************************************************************************/
void Printer::ClearWindows(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t Color) {
  uint16_t X, Y;
  for (Y = YStart; Y < YEnd; Y++) {
    for (X = XStart; X < XEnd; X++) {  //8 pixel =  1 byte
      SetPixel(X, Y, Color);
    }
  }
}

/******************************************************************************
function: Draw Point(XPoint, YPoint) Fill the color
parameter:
    XPoint		: The XPoint coordinate of the point
    YPoint		: The YPoint coordinate of the point
    Color		: Painted color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void Printer::DrawPoint(uint16_t XPoint, uint16_t YPoint, uint16_t Color,
                        DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style) {
  if (XPoint > paint.Width || YPoint > paint.Height) {
    Serial.println("DrawPoint Input exceeds the normal display range\r\n");
    return;
  }

  int16_t XDir_Num, YDir_Num;
  if (Dot_Style == DOT_FILL_AROUND) {
    for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
      for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
        if (XPoint + XDir_Num - Dot_Pixel < 0 || YPoint + YDir_Num - Dot_Pixel < 0)
          break;
        // printf("x = %d, y = %d\r\n", XPoint + XDir_Num - Dot_Pixel, YPoint + YDir_Num - Dot_Pixel);
        SetPixel(XPoint + XDir_Num - Dot_Pixel, YPoint + YDir_Num - Dot_Pixel, Color);
      }
    }
  } else {
    for (XDir_Num = 0; XDir_Num < Dot_Pixel; XDir_Num++) {
      for (YDir_Num = 0; YDir_Num < Dot_Pixel; YDir_Num++) {
        SetPixel(XPoint + XDir_Num - 1, YPoint + YDir_Num - 1, Color);
      }
    }
  }
}

/******************************************************************************
function: Draw a line of arbitrary slope
parameter:
    XStart ：Starting XPoint point coordinates
    YStart ：Starting XPoint point coordinates
    XEnd   ：End point XPoint coordinate
    YEnd   ：End point YPoint coordinate
    Color  ：The color of the line segment
    Line_width : Line width
    Line_Style: Solid and dotted lines
******************************************************************************/
void Printer::DrawLine(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd,
                       uint16_t Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style) {
  if (XStart > paint.Width || YStart > paint.Height || XEnd > paint.Width || YEnd > paint.Height) {
    Serial.println("DrawLine Input exceeds the normal display range\r\n");
    return;
  }

  uint16_t XPoint = XStart;
  uint16_t YPoint = YStart;
  int dx = (int)XEnd - (int)XStart >= 0 ? XEnd - XStart : XStart - XEnd;
  int dy = (int)YEnd - (int)YStart <= 0 ? YEnd - YStart : YStart - YEnd;

  // Increment direction, 1 is positive, -1 is counter;
  int XAddway = XStart < XEnd ? 1 : -1;
  int YAddway = YStart < YEnd ? 1 : -1;

  //Cumulative error
  int Esp = dx + dy;
  char Dotted_Len = 0;

  for (;;) {
    Dotted_Len++;
    //Painted dotted line, 2 point is really virtual
    if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
      //Serial.println("LINE_DOTTED\r\n");
      DrawPoint(XPoint, YPoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
      Dotted_Len = 0;
    } else {
      DrawPoint(XPoint, YPoint, Color, Line_width, DOT_STYLE_DFT);
    }
    if (2 * Esp >= dy) {
      if (XPoint == XEnd)
        break;
      Esp += dy;
      XPoint += XAddway;
    }
    if (2 * Esp <= dx) {
      if (YPoint == YEnd)
        break;
      Esp += dx;
      YPoint += YAddway;
    }
  }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    XStart ：Rectangular  Starting XPoint point coordinates
    YStart ：Rectangular  Starting XPoint point coordinates
    XEnd   ：Rectangular  End point XPoint coordinate
    YEnd   ：Rectangular  End point YPoint coordinate
    Color  ：The color of the Rectangular segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the rectangle
******************************************************************************/
void Printer::DrawRectangle(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd,
                            uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill) {
  if (XStart > paint.Width || YStart > paint.Height || XEnd > paint.Width || YEnd > paint.Height) {
    Serial.println("Input exceeds the normal display range\r\n");
    return;
  }

  if (Draw_Fill) {
    uint16_t YPoint;
    for (YPoint = YStart; YPoint < YEnd; YPoint++) {
      DrawLine(XStart, YPoint, XEnd, YPoint, Color, Line_width, LINE_STYLE_SOLID);
    }
  } else {
    DrawLine(XStart, YStart, XEnd, YStart, Color, Line_width, LINE_STYLE_SOLID);
    DrawLine(XStart, YStart, XStart, YEnd, Color, Line_width, LINE_STYLE_SOLID);
    DrawLine(XEnd, YEnd, XEnd, YStart, Color, Line_width, LINE_STYLE_SOLID);
    DrawLine(XEnd, YEnd, XStart, YEnd, Color, Line_width, LINE_STYLE_SOLID);
  }
}

/******************************************************************************
function: Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the Circle
******************************************************************************/
void Printer::DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius,
                         uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill) {
  if (X_Center > paint.Width || Y_Center >= paint.Height) {
    Serial.println("DrawCircle Input exceeds the normal display range\r\n");
    return;
  }

  //Draw a circle from(0, R) as a starting point
  int16_t XCurrent, YCurrent;
  XCurrent = 0;
  YCurrent = Radius;

  //Cumulative error,judge the next point of the logo
  int16_t Esp = 3 - (Radius << 1);

  int16_t sCountY;
  if (Draw_Fill == DRAW_FILL_FULL) {
    while (XCurrent <= YCurrent) {  //Realistic circles
      for (sCountY = XCurrent; sCountY <= YCurrent; sCountY++) {
        DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //1
        DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //2
        DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //3
        DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //4
        DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //5
        DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //6
        DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  //7
        DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
      }
      if (Esp < 0)
        Esp += 4 * XCurrent + 6;
      else {
        Esp += 10 + 4 * (XCurrent - YCurrent);
        YCurrent--;
      }
      XCurrent++;
    }
  } else {  //Draw a hollow circle
    while (XCurrent <= YCurrent) {
      DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);  //1
      DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);  //2
      DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);  //3
      DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);  //4
      DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);  //5
      DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);  //6
      DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);  //7
      DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);  //0

      if (Esp < 0)
        Esp += 4 * XCurrent + 6;
      else {
        Esp += 10 + 4 * (XCurrent - YCurrent);
        YCurrent--;
      }
      XCurrent++;
    }
  }
}
uint32_t Printer::GetOffset(CodeKind ck, void* ch, void* fo) {
  sFONT* sf;
  char ascii;
  switch (ck) {
    case CT_ENG:
      ascii = *(char*)ch;
      sf = (sFONT*)fo;
      return (ascii - ' ') * sf->Height * (sf->Width / 8 + (sf->Width % 8 ? 1 : 0));
      break;
    default:
      return 0;
      break;
  };
  return 0;
}
void Printer::DrawBuffer(uint16_t XPoint, uint16_t YPoint, const unsigned char* ptr, uint16_t Width, uint16_t Height, uint16_t Color_Foreground, uint16_t Color_Background) {
  uint16_t Page, Column;
  for (Page = 0; Page < Height; Page++) {
    for (Column = 0; Column < Width; Column++) {

      //To determine whether the font background color and screen background color is consistent
      if (FONT_BACKGROUND == Color_Background) {  //this process is to speed up the scan
        if (*ptr & (0x80 >> (Column % 8)))
          SetPixel(XPoint + Column, YPoint + Page, Color_Foreground);
        // DrawPoint(XPoint + Column, YPoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
      } else {
        if (*ptr & (0x80 >> (Column % 8))) {
          SetPixel(XPoint + Column, YPoint + Page, Color_Foreground);
          // DrawPoint(XPoint + Column, YPoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
        } else {
          SetPixel(XPoint + Column, YPoint + Page, Color_Background);
          // DrawPoint(XPoint + Column, YPoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
        }
      }
      //One pixel is 8 bits
      if (Column % 8 == 7)
        ptr++;
    }  // Write a line
    if (Width % 8 != 0)
      ptr++;
  }  // Write all
}
void Printer::DrawBufferSD(uint16_t XPoint, uint16_t YPoint, unsigned long long int pos, File file, uint16_t Width, uint16_t Height, uint16_t Color_Foreground, uint16_t Color_Background) {
  uint16_t Page, Column;
  file.seek(pos);
  uint8_t ptr = file.read();
  for (Page = 0; Page < Height; Page++) {
    for (Column = 0; Column < Width; Column++) {
      //To determine whether the font background color and screen background color is consistent
      if (FONT_BACKGROUND == Color_Background) {  //this process is to speed up the scan
        if (ptr & (0x80 >> (Column % 8)))
          SetPixel(XPoint + Column, YPoint + Page, Color_Foreground);
        // DrawPoint(XPoint + Column, YPoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
      } else {
        if (ptr & (0x80 >> (Column % 8))) {
          SetPixel(XPoint + Column, YPoint + Page, Color_Foreground);
          // DrawPoint(XPoint + Column, YPoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
        } else {
          SetPixel(XPoint + Column, YPoint + Page, Color_Background);
          // DrawPoint(XPoint + Column, YPoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
        }
      }
      //One pixel is 8 bits
      if (Column % 8 == 7)
        ptr = file.read();
    }  // Write a line
    if (Width % 8 != 0)
      ptr = file.read();
  }  // Write all
}
/******************************************************************************
function: Show English characters
parameter:
    XPoint           ：X coordinate
    YPoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Printer::DrawChar(uint16_t XPoint, uint16_t YPoint, const char Acsii_Char, sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background) {
  if (XPoint > paint.Width || YPoint > paint.Height) {
    Serial.println("DrawChar Input exceeds the normal display range\r\n");
    return;
  }
  uint32_t Char_Offset = GetOffset(CT_ENG, (void*)&Acsii_Char, Font);
  const unsigned char* ptr = &Font->table[Char_Offset];
  DrawBuffer(XPoint, YPoint, ptr, Font->Width, Font->Height, Color_Foreground, Color_Background);
}
/******************************************************************************
function:	Display the string
parameter:
    XStart           ：X coordinate
    YStart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Printer::DrawString_EN(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, const char* pString,
                            sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background, int* XNew, int* YNew, bool NextPos, bool NextLine) {
  if (Font == nullptr) return;
  if (XEnd == 0) XEnd = this->paint.Width;
  if (YEnd == 0) YEnd = this->paint.Height;
  if (XStart > paint.Width || YStart > paint.Height || XStart > XEnd || YStart > YEnd) {
    Serial.println("DrawString_EN Input exceeds the normal display range\r\n");
    return;
  }
  uint16_t XPoint = XStart;
  uint16_t YPoint = YStart;
  while (*pString != '\0') {
    if (*pString == '\n') {
      XPoint = XStart;
      YPoint += Font->Height;
      pString++;
      continue;
    }
    if ((XPoint + Font->Width) > XEnd) {
      XPoint = XStart;
      YPoint += Font->Height;
    }
    if ((YPoint + Font->Height) > YEnd) { return; }
    DrawChar(XPoint, YPoint, *pString, Font, Color_Foreground, Color_Background);
    pString++;
    XPoint += Font->Width;
  }
  if (XNew != nullptr) *XNew = XPoint;
  if (YNew != nullptr) {
    *YNew = YPoint;
    if (NextLine) *YNew += Font->Height;
  };
}

void Printer::DrawString_UTF(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, unsigned char* utf8,
                             CFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background, int* XNew, int* YNew, bool NextPos, bool NextLine, uint16_t XLimit) {
  if (Font == nullptr) { return; }
  if (XEnd == 0) XEnd = this->paint.Width - 1;
  if (YEnd == 0) YEnd = this->paint.Height - 1;

  if (XStart > paint.Width || YStart > paint.Height || XStart > XEnd || YStart > YEnd) {
    Serial.println("DrawString_UTF Input exceeds the normal display range\r\n");
    return;
  }

  uint16_t XPoint = XStart;
  uint16_t YPoint = YStart;

  int len = Font->Utf2UniUCS2(utf8, nullptr) * 2;
  unsigned char* uni = new unsigned char[len + 1];
  Font->Utf2UniUCS2(utf8, uni);  //获取unicode码

  if (SysState.GetState(ST_SD_BEGIN) == 0) {
    for (int i = 0; i <= len - 1; i += 2) {  //对每个字符进行输出
      if (uni[i] == 0x00 && uni[i + 1] == '\n') {
        XPoint = XStart;
        YPoint += Font->FontSize;
        continue;
      }
      unsigned long long int pos = Font->GetUnicodePos(uni[i], uni[i + 1]);  //获得区位
      if (pos > Font->FileLength) continue;
      /*unsigned char* buffer = new unsigned char[Font->csize + 1];
      buffer[Font->csize] = '\0';
      Font->FontFile.seek(pos);
      for (int m = 0; m <= Font->csize - 1; m++) { buffer[m] = Font->FontFile.read(); }
      const unsigned char* ptr = &buffer[0];
      DrawBuffer(XPoint, YPoint, ptr, Font->FontSize, Font->FontSize, Color_Foreground, Color_Background);
      delete[] buffer;*/
      DrawBufferSD(XPoint, YPoint, pos, Font->FontFile, Font->FontSize, Font->FontSize, Color_Foreground, Color_Background);
      XPoint += Font->FontSize;
      XPoint -= XLimit;
      if (!NextPos) {
        if (XNew != nullptr) *XNew = XPoint;
        if (YNew != nullptr) *YNew = YPoint;
      }
      if ((XPoint + Font->FontSize - XLimit) > XEnd) {
        XPoint = XStart;
        YPoint += Font->FontSize;
      }
      if ((YPoint + Font->FontSize) > YEnd) { return; }
    }
    if (NextPos) {
      if (XNew != nullptr) *XNew = XPoint;
      if (YNew != nullptr) *YNew = YPoint;
    }
    if (NextLine) *YNew += Font->FontSize;
  }

  else {
    char* ascii = CFONT::ExtractAscii(uni, len / 2);
    Serial.println(ascii);
    Serial.println(strlen(ascii));
    DrawString_EN(XStart, YStart, XEnd, YEnd, ascii, &Font24, Color_Foreground, Color_Background, XNew, YNew, NextPos, NextLine);
  }

  delete[] uni;
  return;
}
void Printer::DisplayString(CodeKind ck, uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, const void* utf, const void* eng,
                            CFONT* cf, sFONT* sf, uint16_t Color_Foreground, uint16_t Color_Background, int* XNew, int* YNew, bool NextPos, bool NextLine, uint16_t XLimit) {
  switch (ck) {
    case CT_ENG:
      DrawString_EN(XStart, YStart, XEnd, YEnd, (char*)eng, (sFONT*)sf, Color_Foreground, Color_Background, XNew, YNew, NextPos, NextLine);
      break;
    case CT_CHN:
      break;
    case CT_UTF:
      DrawString_UTF(XStart, YStart, XEnd, YEnd, (unsigned char*)utf, (CFONT*)cf, Color_Foreground, Color_Background, XNew, YNew, NextPos, NextLine, XLimit);
      break;
    case CT_COM:
      if (cf == nullptr && sf == nullptr) return;
      if (cf != nullptr && SysState.GetState(ST_SD_BEGIN) == 0) DrawString_UTF(XStart, YStart, XEnd, YEnd, (unsigned char*)utf, (CFONT*)cf, Color_Foreground, Color_Background, XNew, YNew, NextPos, NextLine, XLimit);
      else DrawString_EN(XStart, YStart, XEnd, YEnd, (char*)eng, (sFONT*)sf, Color_Foreground, Color_Background, XNew, YNew, NextPos, NextLine);
      break;
    default:
      break;
  }
}

/******************************************************************************
function:	Display nummber
parameter:
    XStart           ：X coordinate
    YStart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
#define ARRAY_LEN 255
void Printer::DrawNum(uint16_t XPoint, uint16_t YPoint, int32_t Nummber,
                      sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background) {

  int16_t Num_Bit = 0, Str_Bit = 0;
  uint8_t Str_Array[ARRAY_LEN] = { 0 }, Num_Array[ARRAY_LEN] = { 0 };
  uint8_t* pStr = Str_Array;

  if (XPoint > paint.Width || YPoint > paint.Height) {
    Serial.println("DisNum Input exceeds the normal display range\r\n");
    return;
  }

  //Converts a number to a string
  while (Nummber) {
    Num_Array[Num_Bit] = Nummber % 10 + '0';
    Num_Bit++;
    Nummber /= 10;
  }

  //The string is inverted
  while (Num_Bit > 0) {
    Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
    Str_Bit++;
    Num_Bit--;
  }

  //show
  DrawString_EN(XPoint, YPoint, 0, 0, (const char*)pStr, Font, Color_Background, Color_Foreground);
}


/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/
void Printer::DrawBitMap(const unsigned char* image_buffer) {
  uint16_t x, y;
  uint32_t Addr = 0;

  for (y = 0; y < paint.HeightByte; y++) {
    for (x = 0; x < paint.WidthByte; x++) {  //8 pixel =  1 byte
      Addr = x + y * paint.WidthByte;
      paint.Image[Addr] = (unsigned char)image_buffer[Addr];
    }
  }
}

/******************************************************************************
function:	Display image
parameter:
    image            ：Image start address
    XStart           : X starting coordinates
    YStart           : Y starting coordinates
    XEnd             ：Image width
    YEnd             : Image height
******************************************************************************/
void Printer::DrawImage(const unsigned char* image_buffer, uint16_t XStart, uint16_t YStart, uint16_t W_Image, uint16_t H_Image) {
  uint16_t x, y;
  uint16_t w_byte = (W_Image % 8) ? (W_Image / 8) + 1 : W_Image / 8;
  uint32_t Addr = 0;
  uint32_t pAddr = 0;
  for (y = 0; y < H_Image; y++) {
    for (x = 0; x < w_byte; x++) {  //8 pixel =  1 byte
      Addr = x + y * w_byte;
      pAddr = x + (XStart / 8) + ((y + YStart) * paint.WidthByte);
      paint.Image[pAddr] = (unsigned char)image_buffer[Addr];
    }
  }
}
/******************************************************************************
function: Display the string
parameter:
    XStart  ：X coordinate
    YStart  ：Y coordinate
    pString ：The first address of the Chinese string and English
              string to be displayed
    Font    ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Printer::DrawString_CN(uint16_t XStart, uint16_t YStart, const char* pString, cFONT* font,
                            uint16_t Color_Foreground, uint16_t Color_Background) {
  const char* p_text = pString;
  int x = XStart, y = YStart;
  int i, j, Num;

  /* Send the string character by character on EPD */
  while (*p_text != 0) {
    if (*p_text <= 0x7F) {  //ASCII < 126
      for (Num = 0; Num < font->size; Num++) {
        if (*p_text == font->table[Num].index[0]) {
          const char* ptr = &font->table[Num].matrix[0];

          for (j = 0; j < font->Height; j++) {
            for (i = 0; i < font->Width; i++) {
              if (FONT_BACKGROUND == Color_Background) {  //this process is to speed up the scan
                if (*ptr & (0x80 >> (i % 8))) {
                  SetPixel(x + i, y + j, Color_Foreground);
                  // DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
              } else {
                if (*ptr & (0x80 >> (i % 8))) {
                  SetPixel(x + i, y + j, Color_Foreground);
                  // DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                  SetPixel(x + i, y + j, Color_Background);
                  // DrawPoint(x + i, y + j, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
              }
              if (i % 8 == 7) {
                ptr++;
              }
            }
            if (font->Width % 8 != 0) {
              ptr++;
            }
          }
          break;
        }
      }
      /* Point on the next character */
      p_text += 1;
      /* Decrement the column position by 16 */
      x += font->ASCII_Width;
    } else {  //Chinese
      for (Num = 0; Num < font->size; Num++) {
        if ((*p_text == font->table[Num].index[0]) && (*(p_text + 1) == font->table[Num].index[1]) && (*(p_text + 2) == font->table[Num].index[2])) {
          const char* ptr = &font->table[Num].matrix[0];

          for (j = 0; j < font->Height; j++) {
            for (i = 0; i < font->Width; i++) {
              if (FONT_BACKGROUND == Color_Background) {  //this process is to speed up the scan
                if (*ptr & (0x80 >> (i % 8))) {
                  SetPixel(x + i, y + j, Color_Foreground);
                  // DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
              } else {
                if (*ptr & (0x80 >> (i % 8))) {
                  SetPixel(x + i, y + j, Color_Foreground);
                  // DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                  SetPixel(x + i, y + j, Color_Background);
                  // DrawPoint(x + i, y + j, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
              }
              if (i % 8 == 7) {
                ptr++;
              }
            }
            if (font->Width % 8 != 0) {
              ptr++;
            }
          }
          break;
        }
      }
      /* Point on the next character */
      p_text += 3;
      /* Decrement the column position by 16 */
      x += font->Width;
    }
  }
}
void Printer::DrawTable_EN(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t MidLine, Table* eng, sFONT* sf, uint16_t Color_Foreground, uint16_t Color_Background) {
  if (eng == nullptr || sf == nullptr) return;
  int ypos1, ypos2, ypos = YStart;
  int size = eng->num;
  for (int i = 0; i <= size - 1; i++) {
    DrawString_EN(XStart, YStart, MidLine, 0, eng->tables[i].content[0].c_str(), sf, Color_Foreground, Color_Background, nullptr, &ypos1, 0, 1);
    DrawString_EN(MidLine, YStart, 0, 0, eng->tables[i].content[1].c_str(), sf, Color_Foreground, Color_Background, nullptr, &ypos2, 0, 1);
    YStart = ypos1 >= ypos2 ? ypos1 : ypos2;
  }
}
void Printer::DrawTable_UTF(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t MidLine, Table* utf, CFONT* cf, uint16_t Color_Foreground, uint16_t Color_Background) {
  if (utf == nullptr || cf == nullptr) return;
  int ypos1, ypos2, ypos = YStart;
  int size = utf->num;
  for (int i = 0; i <= size - 1; i++) {
    DrawString_UTF(XStart, YStart, MidLine, 0, (unsigned char*)utf->tables[i].content[0].c_str(), cf, Color_Foreground, Color_Background, nullptr, &ypos1, 0, 1);
    DrawString_UTF(MidLine, YStart, 0, 0, (unsigned char*)utf->tables[i].content[1].c_str(), cf, Color_Foreground, Color_Background, nullptr, &ypos2, 0, 1);
    YStart = ypos1 >= ypos2 ? ypos1 : ypos2;
  }
}
void Printer::DrawTable(CodeKind ck, uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd, uint16_t MidLine, Table* utf, Table* eng,
                        CFONT* cf, sFONT* sf, uint16_t Color_Foreground, uint16_t Color_Background) {
  int ypos1, ypos2, ypos = YStart;
  int size = 0;
  int mode = 0;
  switch (ck) {
    case CT_ENG:
      DrawTable_EN(XStart, YStart, XEnd, YEnd, MidLine, eng, (sFONT*)sf, Color_Foreground, Color_Background);
      break;
    case CT_CHN:
      break;
    case CT_UTF:
      DrawTable_UTF(XStart, YStart, XEnd, YEnd, MidLine, utf, (CFONT*)cf, Color_Foreground, Color_Background);
      break;
    case CT_COM:
      if (cf == nullptr && sf == nullptr) return;
      if (cf != nullptr && SysState.GetState(ST_SD_BEGIN) == 0 && utf != nullptr) DrawTable_UTF(XStart, YStart, XEnd, YEnd, MidLine, utf, (CFONT*)cf, Color_Foreground, Color_Background);
      else DrawTable_EN(XStart, YStart, XEnd, YEnd, MidLine, eng, (sFONT*)sf, Color_Foreground, Color_Background);
      break;
    default:
      break;
  }
}