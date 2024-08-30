#pragma once
enum KeyName {ST_SD,ST_WIFI,ST_EPD,ST_SD_BEGIN,ST_LITTLEFS};
class State{
  public:
  State();
  short GetState(KeyName name);
  bool SetState(KeyName name,short value);
  private:
  short values[10];
};
extern State SysState;