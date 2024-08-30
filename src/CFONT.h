#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include"State.h"
struct CFONTINFO{
  int FontSize;
  String path;
};
class CFONT {
public:
  CFONT(int FontSize, const char* path);
  void Init(int FontSize, const char* path);
  unsigned long int GetGBKPos(unsigned char* code);
  unsigned long int GetGBK2312Pos(unsigned char* code);
  unsigned long long int GetUnicodePos(unsigned char h, unsigned char l);
  int Utf2UniUCS2One(unsigned char* utf, unsigned char* uni1, unsigned char* uni2);
  int Utf2UniUCS2(unsigned char* utf, unsigned char* uni);
  static char* ExtractAscii(unsigned char* uni,int length);
  void BinaryOut(unsigned char* buffer);
  bool Begin();
  bool GetBuffer(unsigned char* utf, unsigned char* buffer);
  bool WriteBuffer(unsigned char* utf, unsigned char* buffer);
  CFONTINFO Change(int FontSize, const char* path);
  bool End();
  int FontSize = 16;
  uint32_t csize;
  long long int FileLength;
  String path;
  File FontFile;
};
void test(int);
