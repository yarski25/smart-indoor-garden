// -----------------------------
// Smart Indoor Garden
// -----------------------------
// Author: Yaroslav Saprykin
// Date: 13/4/2023
// HW =>
// capacity moisture sensor to detect water tank state
// water pump 5V
// grow lamp
// 2-channel relay to control water pump and grow lamp
// external power supply

#include <Arduino.h>
#include <xxx.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "time.h"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

// HW configs
#define CMS_PIN 33 // Arduino pin that connects to analog pin of capacity moisture sensor (CMS)
#define CMS_AIR 3200 // low level of CMS
#define CMS_WATER 1200 // high level of CMS

// CMS intervals (air, wet, water)
// VERY wET <230;330>
// WET <330; 430>
// DRY <430; 530>

//int intervals = (AIR_VALUE - WATER_VALUE)/3;

#define RELAY_PIN 32  // Arduino pin that connects to relay

unsigned long myTime1;
unsigned long myTime2;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    
    if (text == "/water_status") {

      int waterLevel = analogRead(CMS_PIN);
      //bot.sendMessage(chat_id, "water level value is " + String(waterLevel), "");
      waterLevel = map(waterLevel, CMS_AIR, CMS_WATER, 0, 100);
      bot.sendMessage(chat_id, "water level is " + String(waterLevel) + "% ", "");
      delay(500);
      if (waterLevel > 80){
        bot.sendMessage(chat_id, "water level is HIGH, system OK", "");
      }
      else if( (waterLevel <= 80) && (waterLevel> 50) ){
        bot.sendMessage(chat_id, "water level is MEDIUM, please refill water tank", "");
      }
      else{
        bot.sendMessage(chat_id, "water level is too LOW, please refill water tank", "");
        delay(500);
        bot.sendMessage(chat_id, "irrigation system operation is limited", "");
      }
      delay(500);
    }

    if (text == "/water_pump_on") {

      int waterLevel = analogRead(CMS_PIN);
      waterLevel = map(waterLevel, CMS_AIR, CMS_WATER, 0, 100);
      bot.sendMessage(chat_id, "water level is " + String(waterLevel) + "% ", "");
      delay(500);
      bot.sendMessage(chat_id, "water pump ON", "");
      delay(500);
      myTime1 = millis();
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("water pump ON");
      delay(5000);
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("water pump OFF");
      myTime2 = millis();
      bot.sendMessage(chat_id, "water pump OFF", "");
      delay(500);
      Serial.print("water pump operated for ");
      Serial.print((myTime2-myTime1));
      Serial.println("ms");
      bot.sendMessage(chat_id, "water pump operated for " + String(myTime2-myTime1) + "ms", "");
      delay(60000);
      
      waterLevel = analogRead(CMS_PIN);
      waterLevel = map(waterLevel, CMS_AIR, CMS_WATER, 0, 100);
      bot.sendMessage(chat_id, "water level is " + String(waterLevel) + "% ", "");
      delay(500);
    }

    if (text == "/start")
    {
      String html_msg = "Welcome to <strong>Smart Indoor Garden</strong>, " + from_name + ".\n";
      html_msg += "I'm dog bot and I will help you with this garden.\n\n";
      html_msg += "<a href='/water_status'>/water_status</a> -> <em>returns water tank state in percentage</em>\n";
      html_msg += "<a href='/water_pump_on'>/water_pump_on</a> -> <em>set water pump ON</em>\n";

      bot.sendMessage(chat_id, html_msg, "HTML");
      delay(500);
    }
  }
}

void setTimezone(String timezone){
  //Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void initTime(String timezone){
  struct tm timeinfo;

  //get time via NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("failed to obtain time");
    return;
  }
  // Now we can set the real timezone
  setTimezone(timezone);
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}

void setup() {
  Serial.begin(115200);
  Serial.print("system init...");
  Serial.println("");

  #ifdef ESP8266
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //WiFi.begin(SSID, PASSWORD);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("connecting to WiFi..");
  }
  // print WiFi signal power
  Serial.print("RSSI=");
  Serial.print(WiFi.RSSI());
  Serial.println("dBm");

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Check NTP time
  // Serial.println("");
  initTime(actualTZ);
  printLocalTime();

  bot.sendMessage(CHAT_ID, "bot started", "");
  delay(500);

  pinMode(CMS_PIN, INPUT);
  delay(100);
  //analogReadResolution(12);                   // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
  //delay(100);
  //analogSetWidth(12);                         // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
  //delay(100);

  pinMode(RELAY_PIN, OUTPUT);
  delay(100);
  digitalWrite(RELAY_PIN, HIGH);
  delay(10000);
  Serial.println("system READY...");
  Serial.println("");
}

void loop() {

  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("response received");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // int waterLevel = analogRead(WLS_PIN);
  // delay(1000);
  // waterLevel = map(waterLevel, WLS_LOW, WLS_HIGH, 0, 100);

  // if(waterLevel > 90){
  //   Serial.print("water level HIGH (");
  //   Serial.print(waterLevel);
  //   Serial.println("%)");
  //   myTime1 = millis();
  //   digitalWrite(RELAY_PIN, LOW);
  //   Serial.println("water pump ON");
  //   delay(1000);
  //   digitalWrite(RELAY_PIN, HIGH);
  //   Serial.println("water pump OFF");
  //   myTime2 = millis();
  //   Serial.print("water pump operated for ");
  //   Serial.print((myTime2-myTime1));
  //   Serial.println("ms");
  //   delay(60000);
  // }
  // else{
  //   Serial.print("water level LOW (");
  //   Serial.print(waterLevel);
  //   Serial.println("%)");
  //   digitalWrite(RELAY_PIN, HIGH);
  //   Serial.println("water pump OFF");
  // }

  

//  int value = analogRead(AOUT_PIN); // read the analog value from sensor
//
//  if(value > WATER_VALUE && value < (WATER_VALUE + intervals))
//  {
//    Serial.print("The soil is VERY WET => turn pump OFF (");
//    digitalWrite(RELAY_PIN, LOW);
//  }
//  else if(value > (WATER_VALUE + intervals) && value < (AIR_VALUE - intervals))
//  {
//    Serial.print("The soil is WET => turn pump OFF (");
//    digitalWrite(RELAY_PIN, LOW);
//  }
//  else if(value < AIR_VALUE && value > (AIR_VALUE - intervals))
//  {
//    Serial.print("The soil is DRY (");
//    Serial.print(value);
//    Serial.println(")");
//    digitalWrite(RELAY_PIN, HIGH);
//    Serial.println("pump turn ON");
//    delay(200);
//    digitalWrite(RELAY_PIN, LOW);
//    Serial.println("pump turn OFF");
//    delay(30000);
//  }
//  else
//  {
//    Serial.print("Out of range => turn pump OFF (");
//    digitalWrite(RELAY_PIN, LOW);
//  }
//
//  Serial.print(value);
//  Serial.println(")");
//  delay(500);
}