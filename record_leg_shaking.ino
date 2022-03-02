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
String accX_str;
String accY_str;
String accZ_str;
String gyroX_str;
String gyroY_str;
String gyroZ_str;

// file
File file;
const char* file_name = "/hoge.csv";

// functions
void sync_time();
void fetch_time();
void data_to_string();
void display_data();
void write_data();

// count
int count = 0;

void setup() {
  // set up M5Stack
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

  file = SD.open(file_name, FILE_APPEND);

  M5.Lcd.println("Recoding...");
}

void loop() {
  // get data
  fetch_time();
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);

  data_to_string();

  display_data();

  write_data();

  count++;
  if (count > 200) {
    file.close();
    M5.Lcd.println("Saved.");
  }
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

void data_to_string() {
  accX_str = (String)accX;
  accY_str = (String)accY;
  accZ_str = (String)accZ;
  gyroX_str = (String)gyroX;
  gyroY_str = (String)gyroY;
  gyroZ_str = (String)gyroZ;
}

void display_data() {
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.setCursor(0, 70, 1);

  // time
  M5.Lcd.println("date:  " + date_str + "  ");
  M5.Lcd.println("time:  " + time_str + "  ");

  // acceleration
  M5.Lcd.println("accX:  " + accX_str + "  ");
  M5.Lcd.println("accY:  " + accY_str + "  ");
  M5.Lcd.println("accZ:  " + accZ_str + "  ");

  // gyro
  M5.Lcd.println("gyroX: " + gyroX_str + "  ");
  M5.Lcd.println("gyroY: " + gyroY_str + "  ");
  M5.Lcd.println("gyroZ: " + gyroZ_str + "  ");
}

void write_data() {
  // file = SD.open(file_name, FILE_APPEND);
  file.println(date_str + "," + time_str + "," + accX_str + "," + accY_str + "," + accZ_str + "," + gyroX_str + "," + gyroY_str + "," + gyroZ_str);
  // file.close();
}
