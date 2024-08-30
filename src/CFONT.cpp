#include "CFONT.h"
CFONT::CFONT(int FontSize, const char* path) {
  Init(FontSize,path);
}
void CFONT::Init(int FontSize, const char* path){
  this->path = path;
  this->FontSize = FontSize;
  this->csize = (FontSize / 8 + ((FontSize % 8) ? 1 : 0)) * (FontSize);
  this->FileLength=0;
  SysState.SetState(ST_SD_BEGIN,1);
}
unsigned long int CFONT::GetGBKPos(unsigned char* code) {
  unsigned char qh, ql;
  unsigned long foffset;
  uint8_t csize = (FontSize / 8 + ((FontSize % 8) ? 1 : 0)) * (FontSize);
  qh = *code;
  ql = *(++code);
  if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) {
    return 0; /* �������� */
  }
  if (ql < 0x7f) {
    ql -= 0x40; /* ע��! */
  } else {
    ql -= 0x41;
  }
  qh -= 0x81;
  foffset = ((unsigned long)190 * qh + ql) * csize; /* �õ��ֿ��е��ֽ�ƫ���� */
  return foffset;
}
unsigned long int CFONT::GetGBK2312Pos(unsigned char* code) {
  unsigned char qh = *code, ql = *(1 + code);
  qh -= (0xA0 + 1);
  ql -= (0xA0 + 1);
  return (94 * qh + ql) * csize;
}
unsigned long long int CFONT::GetUnicodePos(unsigned char h, unsigned char l) {
  return (unsigned int)h * csize * 256 + (unsigned int)l * csize;
}
int CFONT::Utf2UniUCS2One(unsigned char* utf, unsigned char* uni1, unsigned char* uni2) {
  char bin = ~utf[0];
  int mode = (bin & 0x80) ? 1 : (bin & 0x20) ? 2
                              : (bin & 0x10) ? 3
                                             : 4;
  switch (mode) {
    case 1:
      *uni1 = 0x00;
      *uni2 = (utf[0] & 0x7F);
      return 1;
      break;
    case 2:
      *uni1 = ((utf[0] & 0x1C) >> 2);
      *uni2 = ((utf[0] & 0x03) << 6) | ((utf[1] & 0x3F));
      return 2;
      break;
    case 3:
      *uni1 = (((utf[0] & 0x0F) << 4) | ((utf[1] & 0x3C) >> 2));
      *uni2 = (((utf[1] & 03) << 6) | (utf[2] & 0x7F));
      return 3;
      break;
    default:
      return 4;
      break;
  }
  return 0;
}
int CFONT::Utf2UniUCS2(unsigned char* utf, unsigned char* uni) {
  int len = strlen((char*)utf);
  int num = 0;
  int p = 0;
  int ex = 1;
  if (uni == nullptr) ex = 0;
  for (int i = 0; i <= len - 1;) {
    unsigned char uni1, uni2;
    i += Utf2UniUCS2One(&utf[i], &uni1, &uni2);
    if (ex) uni[p] = uni1;
    p++;
    if (ex) uni[p] = uni2;
    p++;
    num++;
  }
  return num;
}
char* CFONT::ExtractAscii(unsigned char* uni,int length){
  char* buffer=new char[length+1];
  for(int k=0;k<=length*2-1;k++)Serial.print((char)uni[k]);
  Serial.print(":algikfsd:");
  Serial.println(length);
  int len=0;
  for(int m=0;m<=2*length-1;m+=2){
    if(uni[m]==0x00){
      buffer[len]=(char)uni[m+1];
      len++;
    }
  }
  for(int k=0;k<=len-1;k++)Serial.print((char)buffer[k]);
  char* ascii=new char[len+1];
  for(int n=0;n<=len-1;n++){
    ascii[n]=buffer[n];
  }
  ascii[len]='\0';
  delete[] buffer;
  return ascii;
}
bool CFONT::Begin() {
  if(SysState.GetState(ST_SD_BEGIN)==0)return 0;
  FontFile=SD.open(path,FILE_READ);
  if(SysState.GetState(ST_SD)==0&&FontFile){
    SysState.SetState(ST_SD_BEGIN,0);
    this->FileLength=FontFile.size();
  }
  return FontFile?1:0;
}
bool CFONT::GetBuffer(unsigned char* utf, unsigned char* buffer) {
  int len = Utf2UniUCS2(utf,nullptr) * 2;
  unsigned char* uni = new unsigned char[len + 1];
  Utf2UniUCS2(utf, uni);
  for (int i = 0; i <= len - 1; i += 2) {
    unsigned long int pos = GetUnicodePos(uni[i], uni[i + 1]);
    unsigned char buffer1[129];
    FontFile.seek(pos);
    for (int m = 0; m <= csize - 1; m++) { buffer1[m] = FontFile.read(); }
    BinaryOut(buffer1);
  }
  delete[] uni;
  return true;
}
bool CFONT::End(){
  FontFile.close();
  SysState.SetState(ST_SD_BEGIN,1);
  this->FileLength=0;
  return 1;
}
void CFONT::BinaryOut(unsigned char* buffer) {
  int lines = FontSize;
  int bytesperline = FontSize / 8;
  int byte = 0;
  for (int line = 0; line <= lines - 1; line++) {
    for (int pos = 0; pos <= bytesperline - 1; pos++) {
      for (int i = 7; i >= 0; i--) {
        Serial.print(((buffer[byte] >> i) & 1) ? "##" : "  ");
      }
      byte++;
    }
    Serial.print('\n');
  }
}
CFONTINFO CFONT::Change(int FontSize, const char* path){
  bool st=SysState.GetState(ST_SD_BEGIN);
  CFONTINFO cfinfo={this->FontSize,this->path};
  if(st==0)End();
  Init(FontSize,path);
  if(st==0)Begin();
  return cfinfo;
}
void test(int a){
  CFONT f(16,"UNICODEBASIC16.DZK");
  unsigned char utf[190]="今天";
  Serial.print(a);
  Serial.println("---------");

  f.Begin();
  f.GetBuffer(utf,nullptr);
  //this->print->DrawUTFString(utf, 0, 0, 65, 65, &f, BLACK,WHITE);
  f.End();
}