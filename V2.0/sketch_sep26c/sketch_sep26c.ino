/*
   2017 September 30
   Load Cell HX711 Module Interface with Arduino to measure weight in Kgs
  Arduino
  pin
  2 -> HX711 CLK
  3 -> DOUT
  5V -> VCC
  GND -> GND

  Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.


  HX711
  A0 -> CLK
  A1 -> DOUT

  LCD
  A4 -> SDA
  A5 -> SCL


  Power Supply
  LCD       5V
  HX711   3.3V
*/

#include "HX711.h"  //You must have this library in your arduino library folder
#include <Wire.h>


#define DOUT  3
#define CLK  2

HX711 scale(A3, A2);

float calibration_factor = 100000;
float unit = 0;
bool T = false;
//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  scale.set_scale(calibration_factor);
  scale.tare();
  delay(3000);
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  unit = scale.get_units();
  Serial.print(unit);
  delay(300);
  if (T)
  {
    digitalWrite(13, HIGH);
    T = false;
  }
  else
  {
    digitalWrite(13, LOW);
    T = true;
  }
}
//=============================================================================================
