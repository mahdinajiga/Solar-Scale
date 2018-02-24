/*
   circuits4you.com
   2016 November 25
   Load Cell HX711 Module Interface with Arduino to measure weight in Kgs
  Arduino
  pin
  2 -> HX711 CLK
  3 -> DOUT
  5V -> VCC
  GND -> GND

  Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/

#include "HX711.h"  //You must have this library in your arduino library folder

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define DOUT  3
#define CLK  2
int indexx = 0;
float unit = 0, cal = 0;

HX711 scale(DOUT, CLK);

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 20000; //-106600 worked for my 40Kg max scale setup

//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  lcd.begin(16, 2);
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  unit = scale.get_units();
  indexx++;
  Serial.print("Reading: ");
  Serial.print(unit, 3);
  Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.print(" indexx: ");
  Serial.print(indexx);
  Serial.println();
  if (indexx > 10)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(unit);
    lcd.setCursor(0, 1);
    lcd.print(calibration_factor);
    indexx = 0;
  }
  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if (temp == 's')
      calibration_factor += 100;
    else if (temp == 'x')
      calibration_factor -= 100;
    else if (temp == 'd')
      calibration_factor += 1000;
    else if (temp == 'c')
      calibration_factor -= 1000;
    else if (temp == 'f')
      calibration_factor += 10000;
    else if (temp == 'v')
      calibration_factor -= 10000;
    else if (temp == 't')
      scale.tare();  //Reset the scale to zero
  }
}
//=============================================================================================
