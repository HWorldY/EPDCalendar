#include "WiFiController.h"
#include "EPDHtml.h"
#include "Schedule.h"
#include "State.h"
#include "EPD_INCLUDE.h"
#define MIN 6e7
//TODO:Connect SD3 with VCC through a button(S1),then connect D0 with RST through the button(S1).
//When S1 is on,inttrupt,then go to deepsleep.Every time it wakes up,read pin SD3 to know
//wheather S1 is on or not,which will decide to sleep or run the program.
//Connect RST to VCC through a button to wake it up at ant time(S2).
void InitSerial();
void InitSD();
void InitWiFi();
void InitWiFiServer();
bool InitLittleFS();
RECT screen = { 0, 0, EPD_4IN2_HEIGHT, EPD_4IN2_WIDTH };
ScheduleApp app(screen, ID_MAINAPP);
ICACHE_RAM_ATTR void ReadBusy() {
  Serial.println(digitalRead(EPD_BUSY_PIN) ? "e-paper busy release" : "e-paper busy");
}

ESP8266WebServer server(80);
WiFiController WiFiCtr(&server);
void setup(void) {
  pinMode(D1, INPUT);
  InitSerial();
  Serial.println("starting");
  Serial.println(digitalRead(D1));
  delay(1000);
  if (digitalRead(D1) == HIGH) {

    InitLittleFS();
    InitSD();

    WiFiCtr.InitAP("EPD", "12345678");
    InitWiFiServer();
    WiFiCtr.InitWiFi(WIFI_STA);

    app.WiFiCtr = &WiFiCtr;
    app.server = &server;
    app.Run(ACT_INIT);
    attachInterrupt(EPD_BUSY_PIN, ReadBusy, CHANGE);
    app.Run(ACT_MAIN);
    app.sd->End();

    detachInterrupt(EPD_BUSY_PIN);
    WiFiCtr.mode(WIFI_OFF);
    SPI.end();
    SD.end();
  }
  Serial.println("sleep");
  delay(1000);
  ESP.deepSleep(MIN);
}

void loop(){};

void handleRoot();
void handleQuery();
void handleQueryDate();
void handleQueryDateAll();
void handleRefresh();
void handleAddText();
void handleNotFound();
void handleRemove();
void handleRemoveItem();
void handleSleep();
void handleSave();

void InitSerial() {
  Serial.begin(115200);
}
bool InitLittleFS() {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    SysState.SetState(ST_LITTLEFS, 1);
    return 0;
  }
  SysState.SetState(ST_LITTLEFS, 0);
  return 1;
}
void InitSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    SysState.SetState(ST_SD, 1);
    return;
  }
  SysState.SetState(ST_SD, 0);
  Serial.println("initialization done.");
}
void InitWiFiServer() {
  server.on("/", handleRoot);
  server.on("/query", handleQuery);
  server.on("/update", handleAddText);
  server.on("/querydate", handleQueryDate);
  server.on("/queryall", handleQueryDateAll);
  server.on("/remove", handleRemove);
  server.on("/removeitem", handleRemoveItem);
  server.on("/refresh", handleRefresh);
  server.on("/sleep", handleSleep);
  server.on("/save", handleSave);
  server.on("/reread", handleReread);
  server.onNotFound(handleNotFound);
  //server.begin();
  //Serial.println("HTTP server started");
}
void handleRoot() {
  server.send(200, "text/html", ReadHomePage());  //！！！注意返回网页需要用"text/html" ！！！
  Serial.println("用户访问了主页");
}
void handleQuery() {
  server.send(200, "text/html", ReadQueryPage());  //！！！注意返回网页需要用"text/html" ！！！
  Serial.println("用户查询中...");
}
void handleQueryDate() {
  Date d = Trans(server.arg("date"));
  server.send(200, "text/plain", app.schedule.Print(d));
}
void handleQueryDateAll() {
  server.send(200, "text/plain", app.schedule.PrintAll());
}
void handleRefresh() {
  bool refresh = app.print->IsIdle();
  if (refresh) app.FindChild(ID_SCHEDULE)->Callback(MSG_PAINT);
  server.send(200, "text/plain", (refresh ? "刷新中..." : "刷新失败：墨水屏正在刷新") + GetLocalTime(app.RTC->now()));
}
void handleAddText() {
  String TimeStamp = GetLocalTime(app.RTC->now());
  delay(2000);
  Date d1 = Trans(server.arg("startdate"));
  Date d2 = Trans(server.arg("enddate"));
  String str = server.arg("todo");
  String tool = server.arg("tool");
  int value = server.arg("value").toInt();
  if (tool == "day") {
    server.send(200, "text/plain", "成功发送" + str + (String)'-' + TimeStamp);
    app.schedule.Add(d1, d2, DAY, value, (string)str.c_str());
    delay(10000);
    Serial.println(str);
  } else if (tool == "week") {
    server.send(200, "text/plain", "成功发送" + str + (String)'-' + TimeStamp);
    app.schedule.Add(d1, d2, WEEK, value, (string)str.c_str());
  } else Serial.println("???????");
}

void handleNotFound() {
  server.send(404, "text/html", ReadNotFoundPage());
  Serial.println("用户访问了一个不存在的页面");
}
void handleRemove() {
  server.send(200, "text/html", ReadRemovePage());
  Serial.println("用户正在移除");
}
void handleRemoveItem() {
  String item = server.arg("item");
  app.schedule.RemoveRule(item);
  server.send(200, "text/html", "已移除" + item + GetLocalTime(app.RTC->now()));
}
void handleSleep() {
  app.Callback(MSG_SLEEP);
}
void handleSave() {
  app.schedule.Save();
  server.send(200, "text/html", (String) "已保存" + GetLocalTime(app.RTC->now()));
}
void handleReread() {
  app.schedule.Update();
  server.send(200, "text/html", (String) "已更新" + GetLocalTime(app.RTC->now()));
}
