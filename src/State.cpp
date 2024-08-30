#include "State.h"
State::State(){
  for(int i=0;i<=9;i++){
    values[i]=1;
  }
}
short State::GetState(KeyName name) {
  if (name >= 10 || name <= -1) return -1;
  return values[name];
}
bool State::SetState(KeyName name,short value){
  if (name >= 10 || name <= -1) return 0;
  else{
    values[name]=value;
    return 1;
  }
}
State SysState;