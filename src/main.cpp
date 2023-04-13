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

//#define AOUT_PIN A0 // Arduino pin that connects to AOUT pin of moisture sensor
//#define AIR_VALUE 530 // CMS value air
//#define WATER_VALUE 230 // CMS value water

//#define RELAY_PIN    2  // Arduino pin that connects to relay

// CMS intervals (air, wet, water)
// VERY wET <230;330>
// WET <330; 430>
// DRY <430; 530>

//int intervals = (AIR_VALUE - WATER_VALUE)/3;

void setup() {
  Serial.begin(115200);
  //pinMode(RELAY_PIN, OUTPUT);
  //digitalWrite(RELAY_PIN, LOW);
  //delay(5000);
}

void loop() {

  Serial.print("test msg...");
  delay(5000);
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