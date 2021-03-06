/*
This is the code for the AirGradient DIY Air Quality Sensor with an ESP8266 Microcontroller.

It is a high quality sensor showing PM2.5, CO2, Temperature and Humidity on a small display and can send data over Wifi.

For build instructions please visit https://www.airgradient.com/diy/

Compatible with the following sensors:
Plantower PMS5003 (Fine Particle Sensor)
SenseAir S8 (CO2 Sensor)
SHT30/31 (Temperature/Humidity Sensor)

Dependent Libraries:
The codes needs the following libraries installed:
ESP8266 board with standard libraries
WifiManager by tzar, tablatronix tested with Version 2.0.3-alpha
ESP8266 and ESP32 OLED driver for SSD1306 displays by ThingPulse, Fabrice Weinberg tested with Version 4.1.0

Configuration:
Please set in the code below which sensor you are using and if you want to connect it to WiFi.

If you are a school or university contact us for a free trial on the AirGradient platform.
https://www.airgradient.com/schools/

MIT License
*/

#include <AirGradient.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Wire.h>
#include "SSD1306Wire.h"

AirGradient ag = AirGradient();

SSD1306Wire display(0x3c, SDA, SCL);

// set sensors that you do not use to false
boolean hasPM=true;
boolean hasCO2=true;
boolean hasSHT=true;

// set to true if you want to connect to wifi. The display will show values only when the sensor has wifi connection
boolean connectWIFI=true;

// change if you want to send the data to another server
String POSTURL = "http://192.168.1.142:8086/write?db=air&precision=s";

void setup(){
  Serial.begin(9600);

  display.init();
  display.flipScreenVertically();
  showTextRectangle("Init", String(ESP.getChipId(),HEX),true);

  if (digitalRead(13)) {
    connectWIFI = true;
    showTextRectangle("WIFI", "YES", false);
  } else {
    connectWIFI = false;
    showTextRectangle("WIFI", "NO", false);
  }
  
  if (hasPM) ag.PMS_Init();
  if (hasCO2) ag.CO2_Init();
  if (hasSHT) ag.TMP_RH_Init(0x44);

  if (connectWIFI) connectToWifi();
  delay(2000);
}

void loop(){

  if (hasPM) {
    int PM2 = ag.getPM2_Raw();
    if (connectWIFI) sendData("pm2 value=" + String(PM2));
    showTextRectangle("PM2",String(PM2),false);
    delay(3000);
  }

  if (hasCO2) {
    int CO2 = ag.getCO2_Raw();
    if (connectWIFI) sendData("co2 value=" +String(CO2));
    showTextRectangle("CO2",String(CO2),false);
    delay(3000);
  }

  if (hasSHT) {
    TMP_RH result = ag.periodicFetchData();
    if (connectWIFI) sendData("tmp value=" +String(result.t));
    if (connectWIFI) sendData("hum value=" +String(result.rh));
    showTextRectangle(String(result.t),String(result.rh)+"%",false);
    delay(3000);
  }

}

void sendData(String payload) {
  HTTPClient http;
  http.begin(POSTURL);
  http.POST(payload);
  http.end();
}

// DISPLAY
void showTextRectangle(String ln1, String ln2, boolean small) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  if (small) {
    display.setFont(ArialMT_Plain_16);
  } else {
    display.setFont(ArialMT_Plain_24);
  }
  display.drawString(32, 16, ln1);
  display.drawString(32, 36, ln2);
  display.display();
}

// Wifi Manager
void connectToWifi(){
  WiFiManager wifiManager;
  //WiFi.disconnect(); //to delete previous saved hotspot
  String HOTSPOT = "AIRGRADIENT-"+String(ESP.getChipId(),HEX);
  wifiManager.setTimeout(120);
  if(!wifiManager.autoConnect((const char*)HOTSPOT.c_str())) {
      showTextRectangle("WIFI", "FAIL", false);
      delay(3000);
      ESP.restart();
      delay(5000);
  }
  showTextRectangle("WIFI", "CONN", false);

}
