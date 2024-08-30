#include"AnalogKey.h"
AnalogKey::AnalogKey(int key){
  pinMode(key,INPUT);
  this->key=key;
}
bool AnalogKey::OutOfRange(int value){return value<=500;}//上升沿...
KEYINFO AnalogKey::GetKey(){
  int value=analogRead(key);
  bool outofrange=OutOfRange(value);
  KEYINFO kinfo;
  if(outofrange==true)return kinfo;
  int pressTime=millis();
  while(!OutOfRange(analogRead(key)))delay(1);
  int i=0;
  for(;i<=KeyNum;i++)if(value>=Range[i]&&value<=Range[i+1])break;
  int releaseTime=millis();
  kinfo.id=i;
  kinfo.mode=(releaseTime-pressTime>LongPressTime)?MODE_LONG:MODE_SHORT;
  return kinfo;
}