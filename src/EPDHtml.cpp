#include "EPDHtml.h"
String ReadPage(const char* path) {
  String res;
  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println((String) "Failed to open" + (String)path);
    return String();
  }
  while (file.available()) {
    res +=(char) file.read();
  }
  file.close();
  return res;
}
String ReadHomePage() {
  return ReadPage("/HomePage.html");
}

String ReadQueryPage() {
  return ReadPage("/QueryPage.html");
}
String ReadRemovePage() {
  return ReadPage("/RemovePage.html");
}
String ReadNotFoundPage() {
  return ReadPage("/NotFoundPage.html");
}