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

//#include "HX711.h"  //You must have this library in your arduino library folder
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DOUT  3
#define CLK  2

//HX711 scale(A1, A0);

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 20000; //-106600 worked for my 40Kg max scale setup

int ShowIndex = 0;
int MaxIndex = 9;
int ResInd = 0;
float unit = 0;
int TTare = 0;
bool debugMod = false;
int pinUp = 9, pinDown = 10, pinLCD = 11, pinTare = 12;
String Splash = "Fehhle co";
String Content[10] = {"Sun     khorshid",
                      "Mercury   atarod",
                      "Venus      zohre",
                      "Earth      zamin",
                      "Moon         mah",
                      "Mars     merrikh",
                      "Jupiter moshtari",
                      "Saturn     zohal",
                      "Uranus  oranus!!",
                      "Neptune nepton!!"
                     };
String Content2[10] = {"G:27.94",
                       "G:0.38",
                       "G:0.90",
                       "G:1.00",
                       "G:0.16",
                       "G:0.37",
                       "G:2.52",
                       "G:1.06",
                       "G:0.86",
                       "G:1.14"
                      };
float G[10] = {27.94,
               00.38,
               00.90,
               01.00,
               00.16,
               00.37,
               02.52,
               01.06,
               00.86,
               01.14
              };


//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  EEPROM.begin();
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  pinMode(pinLCD, INPUT_PULLUP);
  pinMode(pinTare, INPUT_PULLUP);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Splash);
  float Q = 0;
  EEPROM.get(0, Q);
  if (Q > 100)
    calibration_factor = Q;
  delay(3000);
  lcd.clear();
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  if (Serial.available() > 0)
  {
    unit = Serial.readString().toInt();
    unit = (100000 / calibration_factor) * unit;
  }
  if (!debugMod)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Content[ShowIndex]);
    lcd.setCursor(0, 1);
    lcd.print(Content2[ShowIndex]);
    lcd.setCursor(8, 1);
    float ttt = unit - TTare;
    ttt = unit * G[ShowIndex];
    lcd.print(ttt, 1);
    lcd.setCursor(14, 1);
    lcd.print("Kg");
  }

  if (digitalRead(pinUp) == LOW)
  {
    //    Serial.println("Up");
    int Y = 0;
    while (digitalRead(pinUp) == LOW)
    {
      Y++;
      delay(1);
    }
    if (Y < 1000)
    {
      if (!debugMod)
      {
        if (ShowIndex == 0)
        {
          ShowIndex = MaxIndex;
        }
        else
        {
          ShowIndex--;
        }
      }
      else
      {
        calibration_factor += 10;
        EEPROM.put(0, calibration_factor);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(calibration_factor);
        lcd.setCursor(0, 1);
        lcd.print(unit);
      }
    }
    else
    {
      debugMod = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(calibration_factor);
      lcd.setCursor(0, 1);
      lcd.print(unit);
    }
  }
  if (digitalRead(pinDown) == LOW)
  {
    //    Serial.println("Down");
    int Y = 0;
    while (digitalRead(pinDown) == LOW)
    {
      Y++;
      delay(1);
    }
    if (Y < 1000)
    {
      if (!debugMod)
      {
        if (ShowIndex == MaxIndex)
        {
          ShowIndex = 0;
        }
        else
        {
          ShowIndex++;
        }
      }
      else
      {
        calibration_factor -= 10;
        EEPROM.put(0, calibration_factor);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(calibration_factor);
        lcd.setCursor(0, 1);
        lcd.print(unit);
      }
    }
    else
    {
      debugMod = false;
    }
  }
  if (digitalRead(pinLCD) == LOW)
  {
    //    Serial.println("LCD");
    if (!debugMod)
    {
      lcd.backlight();
    }
    else
    {
      calibration_factor += 100;
      EEPROM.put(0, calibration_factor);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(calibration_factor);
      lcd.setCursor(0, 1);
      lcd.print(unit);
    }
  }
  if (digitalRead(pinTare) == LOW)
  {
    //    Serial.println("Tare");
    if (!debugMod)
    {
      TTare = unit;
    }
    else
    {
      calibration_factor -= 100;
      EEPROM.put(0, calibration_factor);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(calibration_factor);
      lcd.setCursor(0, 1);
      lcd.print(unit);
    }
  }
  ResInd++;
}
//=============================================================================================
