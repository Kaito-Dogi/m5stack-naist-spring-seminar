#define M5STACK_MPU6886
#include <M5Stack.h>
#include <WiFi.h>
#include <time.h>

// WiFi credentials
const char* ssid = "SSID";
const char* pass = "PASS";

// time
const char* ntp_server = "ntp.jst.mfeed.ad.jp";
const long gmt_offset_sec = 9 * 3600;
const int day_light_offset_sec = 0;
struct tm time_info;
String date_str;
String time_str;

// data
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

// file
File file;
const char* file_name = "/hoge_log.csv";

// functions
void sync_time();
void fetch_time();
void display_data();
void write_data();

void setup() {
  // set up M5Stack.
  M5.begin();
  M5.Power.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.println("CSV Writer");

  M5.IMU.Init();

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
  fetch_time();

  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);

  display_data();
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

void display_data() {
  M5.Lcd.setTextColor(GREEN, BLACK);

  // time
  M5.Lcd.setCursor(0, 50, 1);
  M5.Lcd.println(date_str + "  ");
  M5.Lcd.println(time_str + "  ");

  // acceleration
  M5.Lcd.setCursor(0, 100, 1);
  M5.Lcd.println(accX + "  ");
  M5.Lcd.println(accY + "  ");
  M5.Lcd.println(accZ + "  ");

  // gyro
  M5.Lcd.setCursor(0, 150, 1);
  M5.Lcd.println(gyroX + "  ");
  M5.Lcd.println(gyroY + "  ");
  M5.Lcd.println(gyroZ + "  ");
}



void write_data() {
  file = SD.open(file_name, FILE_APPEND);
  file.println(date_str + "," + time_str
    + "," + accX
    + "," + accX
    + "," + accX
    + "," + gyroX
    + "," + gyroY
    + "," + gyroZ
    );
  file.close();
}
