#include "GetDate.h"
String GetDate() {
  WiFiClient wifiClient;
  HTTPClient http;
  String GetUrl;
  String response;
  GetUrl = "http://quan.suning.com/getSysTime.do";
  http.setTimeout(5000);
  http.begin(wifiClient, GetUrl);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      //读取响应内容
      response = http.getString();
      Serial.println(response);
    }
  } else {
    int count = 0;
    while (http.GET() != HTTP_CODE_OK && count <= 5) {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }
  http.end();
  return response;
}