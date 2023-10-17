#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Replace with your network credentials
const char *ssid = "Test";
const char *password = "9597834243";
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
//Month names
String months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };


void setup() {

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  Serial.begin(115200);
  //Connect to Wi - Fi 
  Serial.print("Connecting to ");
  display.clearDisplay();
  display.setTextSize(2);     // Normal 1:1 pixel scale
  display.setTextColor(WHITE);  // Draw white text
  display.setCursor(0, 24);
  display.println("Connecting to");
  //display.clearDisplay();
  //display.setTextSize(1.5);     // Normal 1:1 pixel scale
  //display.setTextColor(WHITE);  // Draw white text
  display.setCursor(0, 30);
  display.println(ssid);
  display.display();


  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(19803);
}
void loop() {
  Timestamp();
}

void Timestamp() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentMonthName = months[currentMonth - 1];
  String formattedTime = timeClient.getFormattedTime();
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
 // delay(2000);
  String Timest = String(currentDate) + " " + String(formattedTime);
  display.clearDisplay();
  display.setTextSize(2);     // Normal 1:1 pixel scale
  display.setTextColor(WHITE);  // Draw white text
  display.setCursor(205, 5);    // Start at top-left corner
  display.println(Timest);
  display.display();
  // return Timestamp;
}
