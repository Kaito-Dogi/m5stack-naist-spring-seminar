#include <M5Stack.h>
#include <WiFi.h>
#include <time.h>

// WiFi credentials
const char* ssid = "SSID";
const char* pass = "PASS";

// time
char ntp_server[] = "ntp.jst.mfeed.ad.jp";
const long gmt_offset_sec = 9 * 3600;
const int day_light_offset_sec = 0;
struct tm time_info;
String date_str;
String time_str;

// file
File file;
const char* file_name = "/hoge_log.csv";

// functions
void sync_time();
void fetch_time();
void display_time();
void write_data();

void setup() {
  // set up M5Stack.
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.println("CSV Writer");

  // set up WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nWiFi connected.");

  sync_time();
}

void loop() {
  M5.Lcd.fillScreen(BLACK);

  fetch_time();
  display_time();

  write_data();
}

void sync_time() {
  configTime(gmt_offset_sec, day_light_offset_sec, ntp_server);
  while (!getLocalTime(&time_info)) {
    delay(1000);
  }
}

void fetch_time() {
  getLocalTime(&time_info);
  date_str = (String)(time_info.tm_year + 1900) + "/" + (String)(time_info.tm_mon + 1) + "/" + (String)time_info.tm_mday;
  time_str = (String)time_info.tm_hour + ":" + (String)time_info.tm_min + ":" + (String)time_info.tm_sec;
}

void display_time() {
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0, 50, 1);
  M5.Lcd.println(date_str + "  ");
  M5.Lcd.println(time_str + "  ");
}

void write_data() {
  file = SD.open(file_name, FILE_APPEND);
  file.println(date_str + "," + time_str);
  file.close();
}
