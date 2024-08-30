#include"ArduinoApp.h"
ArduinoApp::ArduinoApp(RECT screen,short ID,ArduinoApp* parent){
  this->ID=ID;
  this->screen=screen;
  this->parent=parent;
}
ArduinoApp* ArduinoApp::GetParent(){
  return this->parent;
}
AVALUE ArduinoApp::Callback(short msg,AVALUE param1,AVALUE param2){
  return 0;
}
AVALUE ArduinoApp::Run(short act,AVALUE param1,AVALUE param2){
  return 0;
}
ArduinoApp* ArduinoApp::FindChild(short ID){
  return nullptr;
}