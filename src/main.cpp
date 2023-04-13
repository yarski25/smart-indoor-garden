// -----------------------------
// Smart Indoor Garden
// -----------------------------
// Author: Yaroslav Saprykin
// Date: 13/4/2023
// HW =>
// watel level sensor to detect water tank state
// water pump 5V
// grow lamp
// 2-channel relay to control water pump and grow lamp
// external power supply

#include <Arduino.h>

#define WLS_PIN 25 // Arduino pin that connects to analog pin of water level sensor (WLS)
#define WLS_LOW 0 // low level of WLS
#define WLS_HIGH 2400 // high level of WLS

#define RELAY_PIN 12  // Arduino pin that connects to relay

unsigned long myTime1;
unsigned long myTime2;

// CMS intervals (air, wet, water)
// VERY wET <230;330>
// WET <330; 430>
// DRY <430; 530>

//int intervals = (AIR_VALUE - WATER_VALUE)/3;

void setup() {
  Serial.begin(115200);
  Serial.print("system init...");

  pinMode(WLS_PIN, INPUT);
  delay(100);
  //analogReadResolution(12);                   // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
  //delay(100);
  //analogSetWidth(12);                         // Sets the sample bits and read resolution, default is 12-bit (0 - 4095), range is 9 - 12 bits
  //delay(100);

  pinMode(RELAY_PIN, OUTPUT);
  delay(100);
  digitalWrite(RELAY_PIN, HIGH);
  delay(10000);
  Serial.println("");
  Serial.print("system READY...");
  delay(1000);
  Serial.println("");
}

void loop() {

  int waterLevel = analogRead(WLS_PIN);
  delay(1000);
  waterLevel = map(waterLevel, WLS_LOW, WLS_HIGH, 0, 100);

  if(waterLevel > 90){
    Serial.print("water level HIGH (");
    Serial.print(waterLevel);
    Serial.println("%)");
    myTime1 = millis();
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("water pump ON");
    delay(1000);
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("water pump OFF");
    myTime2 = millis();
    Serial.print("water pump operated for ");
    Serial.print((myTime2-myTime1));
    Serial.println("ms");
    delay(60000);
  }
  else{
    Serial.print("water level LOW (");
    Serial.print(waterLevel);
    Serial.println("%)");
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("water pump OFF");
  }

  

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