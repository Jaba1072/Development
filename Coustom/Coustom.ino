#include <Arduino.h>
#include <Hash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
//#include <AsyncTCP.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
 String retrievedString;
void writeString(char add, String data);
String read_String(char add);
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";
String inputMessage;
String Text;
AsyncWebServer server(80);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

void writeStringToEEPROM(int addrOffset, const String& strToWrite) {
  byte len = strToWrite.length();

  EEPROM.write(addrOffset, len);

  for (int i = 0; i < len; i++) {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  //data[newStrLen] ; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)

  return String(data);
}


// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <head>
  <body style="background-color:powderblue;">
  </body>
<style>

h1 {text-align: center;}
p {text-align: center;}
div {text-align: center;}
</style>
</head>
  <body>
  <title>Page Title</title>
<h1 style="color:blue;">Smart Badge</h1>
<style>
h2 {text-align: center;}
p {text-align: center;}
div {text-align: center;}
</style>  
   <h2>Text To Display</h2>
  <form action="/get">
   <center>  <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br> 
  </form>
</body></html>)rawliteral";

void writeString(char add, String data) {
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++) {
    EEPROM.write(add + i, data[i]);
  }
  EEPROM.write(add + _size, '\0');  //Add termination null character for String Data
  EEPROM.commit();
}


String read_String(char add) {
  int i;
  char data[100];  //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500)  //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}

/*void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}*/
void badge() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(inputMessage);
  display.display();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  EEPROM.begin(512);
  // writeStringToEEPROM(0, "Jos Tech Services Pvt Ltd");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x32
                                              //display.display();
                                              // delay(1000);
                                              // Clear the buffer.
                                              //display.clearDisplay();
                                              // display.setCursor(0, 0);
  String recivedData;
  recivedData = read_String(10);
  Serial.print(recivedData);
  display.println(recivedData);
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  display.display();
  display.display();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  //server.on("/", handle_OnConnect);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + inputMessage + "<br><a href=\"/\">Return to Home Page</a>");
  });
  //server.onNotFound(notFound);


  
  //  String recivedData;
  //Serial.println(retrievedString);

 // writeStringToEEPROM(0, recivedData);
  //display.println(recivedData);
  //display.display();
  //String retrievedString = readStringFromEEPROM(0);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(retrievedString);

  display.display();

  server.begin();
}

void loop() {
  /*retrievedString = readStringFromEEPROM(0);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(retrievedString);
  display.display();
  Serial.println(retrievedString);*/
   badge() ;
}