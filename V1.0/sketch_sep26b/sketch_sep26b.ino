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
*/

#include "HX711.h"  //You must have this library in your arduino library folder
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 20000; //-106600 worked for my 40Kg max scale setup

int ShowIndex = 0;
int MaxIndex = 10;
int ResInd = 0;
float unit = 0;
bool debugMod = false;
int pinUp = 4, pinDown = 5, pinLCD = 6, pinTare = 3;
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
String Content2[10] = {"27.94 G:",
                       "00.38 G:",
                       "00.90 G:",
                       "01.00 G:",
                       "00.16 G:",
                       "00.37 G:",
                       "02.52 G:",
                       "01.06 G:",
                       "00.86 G:",
                       "01.14 G:"
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
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  pinMode(pinLCD, INPUT_PULLUP);
  pinMode(pinTare, INPUT_PULLUP);
  lcd.begin();
  lcd.backlight();
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  unit = scale.get_units();
  Serial.print(unit, 3);
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  delay(1);
  if (ResInd > 250)
  {
    ResInd = 0;
    if (!debugMod)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(Content[ShowIndex]);
      lcd.setCursor(0, 1);
      lcd.print(Content2[ShowIndex]);
      lcd.setCursor(8, 1);
      float ttt = unit * G[ShowIndex];
      lcd.print(ttt);
      lcd.setCursor(14, 1);
      lcd.print("Kg");
    }
  }
  if (digitalRead(pinUp) == LOW)
  {
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
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(calibration_factor);
      }
    }
    else
    {
      debugMod = true;
    }
  }
  if (digitalRead(pinDown) == LOW)
  {
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
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(calibration_factor);
      }
    }
    else
    {
      debugMod = false;
    }
  }
  if (digitalRead(pinLCD) == LOW)
  {
    if (!debugMod)
    {
      lcd.backlight();
    }
    else
    {
      calibration_factor += 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(calibration_factor);
    }
  }
  if (digitalRead(pinTare) == LOW)
  {
    if (!debugMod)
    {
      scale.tare();
    }
    else
    {
      calibration_factor -= 100;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(calibration_factor);
    }
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
  ResInd++;
}
//=============================================================================================
