#pragma once
#include <SPI.h>
#include <SD.h>
unsigned long int text_get_hz_mat(unsigned char *code, unsigned char *mat,uint8_t size) {
  unsigned char qh, ql;
  unsigned char i;
  unsigned long foffset;
  /* 得到字体一个字符对应点阵集所占的字节数 */
  uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size);
  qh = *code;
  ql = *(++code);
  if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) /* 非 常用汉字 */
  {
    for (i = 0; i < csize; i++) {
      *mat++ = 0x00; /* 填充满格 */
    }
    return 0; /* 结束访问 */
  }
  if (ql < 0x7f) {
    ql -= 0x40; /* 注意! */
  } else {
    ql -= 0x41;
  }
  qh -= 0x81;
  foffset = ((unsigned long)190 * qh + ql) * csize; /* 得到字库中的字节偏移量 */
  return foffset;
}
void Binarycout(char *buffer, int lines, int bytesperline) {
  int byte = 0;
  for (int line = 0; line <= lines - 1; line++) {
    for (int pos = 0; pos <= bytesperline - 1; pos++) {
      for (int i = 7; i >= 0; i--) {
        Serial.print(((buffer[byte] >> i) & 1) ? '*' : ' ');
      }
      byte++;
    }
    Serial.println("");
  }
}
void GetGBKArray(uint8_t* buffer,unsigned char* GBKString,uint8_t size=16){
int len=strlen(GBKString)/2;
File myFile;
myFile = SD.open("SongFont16.DZK", FILE_READ);
int index=0;
if (myFile) {
  for(int chpos=0;chpos<=len-1;chpos++){
  unsigned long int pos = text_get_hz_mat(GBKString[chpos*2], nullptr, size);
  myFile.seek(pos);
  for (int i = size*2*chpos; i <= size*2*(chpos+1) - 1; i++) buffer[i] = myFile.read();}
}
}